#include "mbed.h"
#include "ATP301x_SPI.h"

//public function

    ATP301x_SPI::ATP301x_SPI(PinName mosi, PinName miso, PinName sck, PinName ss):
            _spi(mosi, miso, sck), _ss(ss){
        atp_initialize();
    }
    
    
    void ATP301x_SPI::stop(){
         char stop1 = '>';
         int rx;
    
        //ブレークコマンド $
         rx = _spi.write('$');
         wait_us(SPI_SEND_PERIOD_US);
         
         //breakコマンドに対する応答”E255”がバッファに溜まってるのでダミーデータを送って">"が返ってくるまで取出す
         //データシート「ブレークコマンド」参照
         while(rx != (int)stop1){
            rx = _spi.write(0xFF);
            wait_us(SPI_SEND_PERIOD_US);
         }
    }


    //ATP301xの音声記号を受け取ってしゃべる
    void ATP301x_SPI::talk(char input[], bool useWait){
        _ss = 0;
            
        stop();
        
        bool tooLong = false;
        int i = 0;
            
        while(input[i] != 0xFF && input[i] != '\0' && input[i] != NULL){
            _spi.write(input[i]);
            wait_us(SPI_SEND_PERIOD_US);
                
            //発音記号の終端（ピリオド）を送ったらbreak
            //※while終了条件に組み込むと、Whileの外で.を送ることになりチャイムが鳴らせない
            if(input[i] == '.'){
                break;
            }
                
            i++;
                
            //ATP301xが受信できる最大文字数を超えたら強制終了
            if(i > ATP_MAX_LEN){
                tooLong = true;
                break;
            }
        }

        //ATP301xコマンドの終端を示すキャレッジリターンを送信
        _spi.write('\r');
            
        if(useWait){
            atp_wait();
        }
        
        _ss = 1;
        if(tooLong){
            //音声合成「音声記号が長すぎます。」
            talk("onnse-ki'go-ga/nagasugima'_su.",true);
        }
    }
    
    void ATP301x_SPI::chimeJ(bool useWait){
        talk("#J", useWait);
    }
    
    void ATP301x_SPI::chimeK(bool useWait){
        talk("#K", useWait);
    }

//private functin

    void ATP301x_SPI::atp_initialize(){
        _spi.format(8,3); //bit, mode
        _spi.frequency(SPI_CLK_HZ);
    }
    
    //発音完了まで別の処理をさせたくない場合にwaitでマイコン全体を止める
    //ダミーデータを送信し、Busyである（＝発音中）ならばwait()
    //ATP301xに発話完了時のアクションはなく
    //”Ready待ちはポーリングで行う。”とデータシートに記載あり
    //データはシート「SPI通信」参照
    void ATP301x_SPI::atp_wait(){
        //Busy時の返ってくるはずのレスポンス
        char busyRes = '*';  
        int rx;
              
        //ダミーデータを送信してポーリング
        do{
            wait_ms(POLLING_CYCLE_MS);
            rx = _spi.write(0xFF);
        }while(rx == (int)busyRes);
    }
    

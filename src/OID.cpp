#include <Arduino.h>
/*
OID 相关文档请参阅 零界线硬件开发/装配说明文档/OID光学传感器
OID 最终通过OID_scan()返回的是一个String，包含X,Y坐标以及识别镜头和纸张的角度
例如 “13,22,221" X=13 Y=22 角度是221
OID 型号目前是 松翰科技的 SNM9S5xxxC2100A 系列 
*/
#define OID_USERCMD_POWER_ON				0x0053
#define UserCmd_MinFrameRate_20 0x0024//
#define UserCmd_EnablePositionWorkingMode 0x0040
#define UserCmd_OutputAngleEnable 0x0010
#define oid_sclk    GPIO_NUM_19
#define oid_sdio    GPIO_NUM_5

int OID_X =0,OID_Y =0,OID_Angle=0;
int OID_Index=0;
int temp_X[3],temp_Y[3],temp_Angle[3];
int temp_index[10];
uint8_t temp_debounce=0;
boolean OID_available=false;
boolean OID_code_type=0;//1-手写码 2-普通码

static uint16_t TranCommand;
static uint8_t Muti_Write_Flg;
static uint16_t TranMutiData[6];
static const uint16_t oid_reg_addr1[]={0x0000,0x0000,0x0D40,0x0DC3,0x0000,0x0DC3,0x0000,0x0000,0x0000,0x0D63,0xFFFF};
//UserCmd_PositionEnable 0x0035 
//UserCmd_AutoSwitchToPosEnable 0x0043 

static const uint16_t oid_reg_data1[]={0x004D,0x00D7,0x0000,0x0001,0x003C,0x0000,0x0010,0x0035,0x0043,0x0000,0xFFFF};
static uint16_t OpticalData[4];

void Receive_Data(){
  for(int i=0;i<4;i++){OpticalData[i]=0;}//清空、重置OpticalData
  //起步动作
  gpio_set_level(oid_sclk, 1);// start bit
  gpio_set_direction(oid_sdio, GPIO_MODE_OUTPUT);// give control signal low
  gpio_set_level(oid_sdio, 0);
  delayMicroseconds(5);
  //
  gpio_set_level(oid_sclk, 0);// SCLK low, disable gie, and wait 2us
  delayMicroseconds(5);
  //
  gpio_set_level(oid_sclk, 1);// start bit
  gpio_set_direction(oid_sdio, GPIO_MODE_INPUT);// set as input mode
  delayMicroseconds(2);
  //
  for(int i=0;i<64;i++){
    gpio_set_level(oid_sclk, 1); 
    delayMicroseconds(5);
    gpio_set_level(oid_sclk, 0);// SCLK low, disable gie, and wait 2us
    delayMicroseconds(5);
    if(gpio_get_level(oid_sdio)==1){
      if(i<16)OpticalData[0]+=1;
      else if(i<32)OpticalData[1]+=1;
      else if(i<48)OpticalData[2]+=1;
      else OpticalData[3]+=1;
    }
    if(i<15)OpticalData[0]<<=1;
    else if(i<31)OpticalData[1]<<=1;
    else if(i<47)OpticalData[2]<<=1;
    else if(i<63)OpticalData[3]<<=1;
  }
  delayMicroseconds(79); 

}

unsigned int OID_CheckReadData(){ //检查数据线当前状态，如果是高电平返回1，低电平返回0
  if(gpio_get_level(oid_sdio))return 1;
  delayMicroseconds(5);
  if(gpio_get_level(oid_sdio))return 1;
  return 0;
}

unsigned int OID_ReceiveData(unsigned int *pOpticalData){
  if(OID_CheckReadData()==1)return 1;//第三次确认，是不是SDIO被拉低了
  //如果被拉低了
  Receive_Data();
  if((OpticalData[0]&0x1000)==0){
    for(int i=0;i<4;i++){
      pOpticalData[i]=OpticalData[i];
    }
    return 0;//有效数据被period Optical Data继承了，全局变量的OpticalData可以被随时清空
  }
  return 1;
}

unsigned int OID_PowerOn(void){//return 0-正常启动，1-未启动成功超时 2-校准模式
  unsigned int uiRecData[4];
	unsigned int uiRetry = 0;
  gpio_set_level(oid_sclk, 1);            //set SCLK High	
  delayMicroseconds(30000);	 
  gpio_set_level(oid_sclk, 0);            //set SCLK Low

  while(1){
    if(OID_CheckReadData()==0){
      if(OID_ReceiveData(uiRecData)==0){
        //printf("uiRecData[0]=0x%x, uiRecData[1]=0x%x, uiRecData[2]=0x%x, uiRecData[3]=0x%x\r\n",uiRecData[0], uiRecData[1], uiRecData[2], uiRecData[3]);
        uiRecData[0] &= (unsigned int)0xE000;
        if((uiRecData[0] == (unsigned int)0xA000) && (uiRecData[3] == (unsigned int)0xFFF8)){
          printf("OID Power On Sucessful!!!\r\n");
					break;
        }else if((uiRecData[0] == (unsigned int)0xA000) && (uiRecData[3] == (unsigned int)0xFFF6)){
          printf("OID Goto Calibration Mode\r\n");
					return 2;
        }//校正笔头
      }
    }
    delayMicroseconds(100);
    uiRetry++;
    if(uiRetry>=3000){//300ms
      printf("OID Power On Timer OUt!!!\r\n");
			return 1;
    }
  }
  return 0;
}

void Transmit_Data(){
  gpio_set_direction(oid_sdio, GPIO_MODE_OUTPUT);
  gpio_set_level(oid_sdio,1);
  gpio_set_level(oid_sclk,1);
  delayMicroseconds(100);
  gpio_set_level(oid_sclk, 0);// SCLK low, disable gie, and wait 2us
  delayMicroseconds(5);
  for(int i=0;i<8;i++){
    gpio_set_level(oid_sclk, 1);// clock high then put data and enable gie
    delayMicroseconds(5);
    if((TranCommand & 0x80))gpio_set_level(oid_sdio, 1);//每次和1000 0000 做&位与计算最高位，之后左移最高位 
    else gpio_set_level(oid_sdio, 0);
    gpio_set_level(oid_sclk, 0);// SCLK low, disable gie, and wait 2us
    delayMicroseconds(5);
    TranCommand <<= 1;//从MSB Most significant byte 高位发送，到 LSB 
  }
  gpio_set_direction(oid_sdio, GPIO_MODE_INPUT);
  delayMicroseconds(78);
}

void Receive_Respond(){
  for (int i=0;i<4;i++)OpticalData[i]=0;
  gpio_set_level(oid_sclk, 1);// start bit
  gpio_set_direction(oid_sdio, GPIO_MODE_OUTPUT);
  gpio_set_level(oid_sdio, 0);// give control signal low
  delayMicroseconds(5);
  gpio_set_level(oid_sclk, 0);// SCLK low, disable gie, and wait 2us   
  delayMicroseconds(5);
  gpio_set_level(oid_sclk, 1);// start bit			
  gpio_set_direction(oid_sdio, GPIO_MODE_INPUT);// set as input mode
  delayMicroseconds(2);
  for(int i=0; i<16; i++){
    gpio_set_level(oid_sclk, 1);
    delayMicroseconds(5);
    gpio_set_level(oid_sclk, 0);// SCLK low, disable gie, and wait 2us
    delayMicroseconds(5);
    if(gpio_get_level(oid_sdio))OpticalData[0] += 1;
    if(i<15)OpticalData[0] <<= 1;
  }
  delayMicroseconds(79);
}

unsigned int OID_TransmitCmd(unsigned int uiCmd){
  unsigned int uiRecData[4];
  unsigned int uiRespond;
  unsigned int uiRetry = 0;
  unsigned long TimeOutCount=0;
  unsigned int TestFlag = 0;
  unsigned int Ret=0;
  if(OID_CheckReadData()==0){
    OID_ReceiveData(uiRecData);
  }
  TranCommand = uiCmd & 0x00FF;			//Transmit command to pen
  Transmit_Data();
  if(uiCmd != OID_USERCMD_POWER_ON){
    while(1){
      if(OID_CheckReadData()==0){
        Receive_Respond();
        uiRespond = OpticalData[0];
        if(uiRespond == (uiCmd + 1)){
          TimeOutCount = 0;
          delayMicroseconds(1000);
          Ret = 0;
          break;
        }
      }
      delayMicroseconds(100);
      uiRetry++;
      if(uiRetry >= 3000)return 1;
    }
  }
  if (Muti_Write_Flg){
    Muti_Write_Flg = 0;
    for (int i = 0; i <= TranMutiData[0]; i++){
      TranCommand = TranMutiData[i] & 0x00FF;
      if(TestFlag==1)TranCommand = 0x00;
      Transmit_Data ();
      while(1){
        if(OID_CheckReadData ()==1){
          TimeOutCount++;
          if(TimeOutCount>15000){
            if(TestFlag==1)return 4;
            return 1;
          }
          continue;
        }
        Receive_Respond ();	
        uiRespond = OpticalData[0];
        if((uiRespond & 0x00FF) == ((TranMutiData[i] + 1) & 0x00FF)){
          TimeOutCount = 0;
          delayMicroseconds(1000);
          break;
        }else if ((uiRespond & 0x00FF) == ((TranMutiData[i] - 1) & 0x00FF)){
          return 2;
        }else {
          if (i == 0 && uiRespond == 0xFFFF) return 3;
          TestFlag = 1;
          return 4;
        }
      }
    }
  }
  return Ret;
}

unsigned int Read_Dec_Info (unsigned int Cmd, unsigned int *Data1, unsigned int *Data2){
  unsigned int Ret;
	unsigned int ErrorCount;
	unsigned long TimeOutCount=0;
  Ret = 0xFF;
  ErrorCount = 0;
  if (Cmd == 0x74){
    TranMutiData[0] = 3;
    TranMutiData[1] = *Data1 >> 8;
    TranMutiData[2] = *Data1 & 0x00FF;
    TranMutiData[3] = (TranMutiData[0] + TranMutiData[1] + TranMutiData[2]) & 0xFF;
    while (ErrorCount < 3){
      Muti_Write_Flg = 1;
      Ret = OID_TransmitCmd (Cmd);	//0:success
      if (Ret == 2) ErrorCount++;
      else break;
    }
    Muti_Write_Flg = 0;	
  }else return 4;	//cmd fail

  if (!Ret){
    TimeOutCount = 0;
    while (1){
      if(!(OID_CheckReadData ())){
        Receive_Data ();
        if ((OpticalData[0] &  0xE000) == 0xE000){
          *Data1 = OpticalData[2];
          *Data2 = OpticalData[3];
          return 0;
        }
      }
      delayMicroseconds(100);
      TimeOutCount ++;
      if (TimeOutCount > 3000)return 1;	// Time out = 300ms
    }
  }
  return Ret;
}

unsigned int Write_Dec_Info (unsigned int Cmd, unsigned int *Data1 , unsigned int *Data2)
{
	unsigned int Ret;
	unsigned int ErrorCount;
	Ret = 0xFF;
	ErrorCount = 0;						
	if (Cmd == 0x73)
	{
			TranMutiData[0] = 5;
			TranMutiData[1] = *Data1 >> 8;
			TranMutiData[2] = *Data1 & 0x00FF;
			TranMutiData[3] = *Data2 >> 8;
			TranMutiData[4] = *Data2 & 0x00FF;
			TranMutiData[5] = (TranMutiData[0] + TranMutiData[1] + TranMutiData[2] + \
												TranMutiData[3] + TranMutiData[4]) & 0xFF;
	}
	else
		return 4;	//cmd fail

	while (ErrorCount < 3)
	{
		Muti_Write_Flg = 1;
		Ret = OID_TransmitCmd (Cmd);
		if (Ret == 2) ErrorCount++;
		else break;
	}

	Muti_Write_Flg = 0;
	return Ret;
}

unsigned int DecoderInit(){
  unsigned int i=0;
  unsigned int D11,D22,RetryCnt;
  while(1){
    if(oid_reg_addr1[i] == 0xFFFF) {
			break;
		}else if(oid_reg_addr1[i] == 0x0000) {
			RetryCnt=0;
			while(OID_TransmitCmd(oid_reg_data1[i])) {
				RetryCnt++;
				if(RetryCnt >=3)
					return 1;
			}
		}else if(oid_reg_addr1[i] == 0x0D63) {
      D11 = oid_reg_addr1[i];
      D22 = 0;
      RetryCnt = 0;
      while(Read_Dec_Info(0x74, &D11 , &D22)) {
        RetryCnt++;
        if(RetryCnt >=3)return 1;
      }
      RetryCnt=0;
      D11 = oid_reg_addr1[i];
      D22 = D22&0xFFFE;
      while(Write_Dec_Info(0x73, &D11 , &D22)) {
        RetryCnt++;
        if(RetryCnt >=3)return 1;
      }
    }else {
      RetryCnt=0;
      D11 = oid_reg_addr1[i];
      D22 = oid_reg_data1[i];
      while(Write_Dec_Info(0x73, &D11 , &D22)) {
        RetryCnt++;
        if(RetryCnt >=3)return 1;
      }
    }
    i++;
  }
  return 0;
}

void OID_Release(void)
{
    gpio_set_direction(oid_sclk, GPIO_MODE_INPUT);
    gpio_pullup_dis(oid_sclk);
    gpio_set_direction(oid_sdio, GPIO_MODE_INPUT);
    gpio_pullup_dis(oid_sdio);
}

void OID_Init(){
  gpio_pad_select_gpio(oid_sclk);
  gpio_set_direction(oid_sclk, GPIO_MODE_OUTPUT);
  gpio_set_level(oid_sclk,0);

  gpio_pad_select_gpio(oid_sdio);
  gpio_set_direction(oid_sdio, GPIO_MODE_INPUT);
  gpio_pullup_dis(oid_sdio);

  uint8_t uiTemp;
  unsigned int OID_Receive[4];
  uiTemp = OID_PowerOn();//return 0-正常启动，1-未启动成功超时 2-校准模式
  if(uiTemp<2){
    uiTemp = DecoderInit();
    if(uiTemp==1){printf("OID Initialization Failed!!!\r\n");}
    else printf("OID Initialization OK!!!\r\n");
  }else if(uiTemp == 2){
    //OID_Release();
    //printf("OID IO Release!!!\r\n");
    //while(1)delay(500);
  }

  if(OID_TransmitCmd(UserCmd_MinFrameRate_20)==0)Serial.println("framerate ok!");
  if(OID_TransmitCmd(UserCmd_EnablePositionWorkingMode)==0)Serial.println("posworking ok!");
  if(OID_TransmitCmd(UserCmd_OutputAngleEnable)==0)Serial.println("angle ok!");
}

int mostFrequent(int* arr, int n)
{
    int maxcount = 0;
    int element_having_max_freq;
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (arr[i] == arr[j])
                count++;
        }
 
        if (count > maxcount) {
            maxcount = count;
            element_having_max_freq = arr[i];
        }
    }
 
    return element_having_max_freq;
}

void OID_scan(){
  unsigned int OID_Receive[4];
  String result = "";
  if(OID_CheckReadData()==0){
    if(OID_ReceiveData(OID_Receive)==0){
      if((OID_Receive[0]&0xB000)==0x2000){//是手写码
        temp_X[temp_debounce] = OID_Receive[3]&0x3FFF;
        temp_Y[temp_debounce] = (((OID_Receive[2]<<2)+(OID_Receive[3]>>14))&0x3FFF);
        temp_Angle[temp_debounce] = (((OID_Receive[0]<<3)+(OID_Receive[1]>>13))&0x1FF);
        //Serial.print("temp_X: ");Serial.print(temp_X[temp_debounce]);Serial.print(" temp_Y: ");Serial.print(temp_Y[temp_debounce]);Serial.print(" temp_Angle: ");Serial.println(temp_Angle[temp_debounce]);
        if(temp_X[temp_debounce]>=15000||temp_Y[temp_debounce]>=15000){temp_X[temp_debounce]=0;temp_Y[temp_debounce]=0;}
        temp_debounce++;
        if(temp_debounce>=2){
          if((temp_X[0]==temp_X[1]==temp_X[2])&&(temp_Y[0]==temp_Y[1]==temp_Y[2])){
            if((abs(temp_X[0]-OID_X)>500)||(abs(temp_Y[0]-OID_Y)>500)){
              OID_available=false;
              OID_code_type=0;//无效码
              temp_debounce=0;
            }else{
              OID_X=temp_X[0];
              OID_Y=temp_Y[0];
              OID_Angle=(360-temp_Angle[0]);
              OID_available=true;
              OID_code_type=1;//手写码
            }
          }else {
            OID_available=false;
            OID_code_type=0;//无效码
          }
          temp_debounce=0;
        }
      }else if((OID_Receive[0]&0xB000)==0x0000){//是普通码 这个滤波仍然有问题，还不能100%捕捉到数据
        temp_index[temp_debounce] =OID_Receive[3]&0x3FFF;
        if(temp_debounce<9){
          temp_debounce++;
        }else if(temp_debounce==9){//10次捕获数据之后
          int most_freq_index=mostFrequent(temp_index,10);//得到最常出现的那个数据
          if(OID_Index==0){//如果之前没有捕获过数据，那么先记录这个数据
            OID_available=true;
            OID_code_type=2;//普通码
            OID_Index=most_freq_index;
          }else{
            if(most_freq_index<12000&&most_freq_index!=0){//如果最常出现的数据<12000且有效数据不为0
              OID_available=true;
              OID_code_type=2;//普通码
              OID_Index=most_freq_index;
            }else{
              OID_available=false;
              OID_code_type=0;
            }
          }
          temp_debounce=0;
        }
      }else {OID_available=false;}//这是对于收到错误的数据而言
    }else {OID_available=false;}//对于没有收到数据而言
  }else {OID_available=false;}//对于没有收到数据而言
  
  
}

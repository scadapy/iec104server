#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <signal.h>
#include "cs104_slave.h"
#include "hal_thread.h"
#include "hal_time.h"
#include "parson.h"
#include <malloc.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


static bool running = true;
int scaleVal = 0;

// set discret
int  countOfSinglePoint;

bool  *SinglePointValue;
int   *SinglePointAddress;
char*  SinglePointVarname[100];
int   *SinglePointByteCount;
int   *SinglePointOffset;
int   *SinglePointByteSequence;
bool   *SinglePointNewData;
bool  *SinglePointOldData;

//set mesure
int   countOfMeasureValue;
float   *MeasureValue;
int   *MeasureAddress;
char*  MeasureVarname[100];
int   *MeasureByteCount;
int   *MeasureOffset;
int   *MeasureByteSequence;
float   *MeasureValueOld;
float   *MeasureValueNew;
float   *MeasureValueKoef;
char*  MeasureVarType[100];


void LoadJsonFile(char *jfile)
{
     JSON_Value *root_value;
     JSON_Array *jArray;
     JSON_Object *jObj;
     size_t i;
     int CountArrayJsonFile;

     root_value = json_parse_file(jfile);
     jArray = json_value_get_array(root_value);
     CountArrayJsonFile=json_array_get_count(jArray);

/*   discret */
      for (i = 0; i < json_array_get_count(jArray); i++)
    {
        jObj = json_array_get_object(jArray, i);
        if(json_object_dotget_string(jObj, "SinglePoint.VarName")!=NULL)
        {
            countOfSinglePoint=countOfSinglePoint+1;
        }
    }

     printf("Count Singlepoints: %d\n",countOfSinglePoint);

     SinglePointAddress      = (int *)malloc( countOfSinglePoint*sizeof(int) );
     SinglePointByteCount    = (int *)malloc( countOfSinglePoint*sizeof(int) );
     SinglePointOffset       = (int *)malloc( countOfSinglePoint*sizeof(int) );
     SinglePointByteSequence = (int *)malloc( countOfSinglePoint*sizeof(int) );
     SinglePointValue        = (bool *)malloc( countOfSinglePoint*sizeof(bool) );
     SinglePointNewData      = (bool *)malloc( countOfSinglePoint*sizeof(bool) );
     SinglePointOldData      = (bool *)malloc( countOfSinglePoint*sizeof(bool) );

    // SinglePointVarname =   (char *)malloc( countOfSinglePoint*sizeof(char) );

     printf("\nLoad SinglePoint\n\n");
     int ii=0;
     for (i = 0; i < json_array_get_count(jArray); i++)
    {
        jObj = json_array_get_object(jArray, i);

        if(json_object_dotget_string(jObj, "SinglePoint.VarName")!=NULL)
        {
        printf("%s:%s:%s:%s:%s\n",
               json_object_dotget_string(jObj, "SinglePoint.VarName"),
               json_object_dotget_string(jObj, "SinglePoint.IecAddress"),
               json_object_dotget_string(jObj, "SinglePoint.OffSet"),
               json_object_dotget_string(jObj, "SinglePoint.ByteCount"),
               json_object_dotget_string(jObj, "SinglePoint.ByteSequence"));


         SinglePointVarname[ii]     =json_object_dotget_string(jObj, "SinglePoint.VarName");
         SinglePointAddress[ii]     =atoi(json_object_dotget_string(jObj, "SinglePoint.IecAddress"));
         SinglePointOffset[ii]      =atoi(json_object_dotget_string(jObj, "SinglePoint.OffSet"));
         SinglePointByteCount[ii]   =atoi(json_object_dotget_string(jObj, "SinglePoint.ByteCount"));
         SinglePointByteSequence[ii]=atoi(json_object_dotget_string(jObj, "SinglePoint.ByteSequence"));
         SinglePointNewData[ii]     =false;
         SinglePointOldData[ii]     =false; 
         SinglePointValue[ii]       =false;
         ii=ii+1;

        }
    }

/* mesure values*/
     printf("\nLoad MeasureValue\n\n");
      for (i = 0; i < json_array_get_count(jArray); i++)
    {
        jObj = json_array_get_object(jArray, i);
        if(json_object_dotget_string(jObj, "MeasureValue.VarName")!=NULL)
        {
            countOfMeasureValue=countOfMeasureValue+1;
        }
    }
     printf("Count MeasureValue: %d\n",countOfMeasureValue);
     MeasureAddress      =(int *)malloc( countOfMeasureValue*sizeof(int) );
     MeasureByteCount    =(int *)malloc( countOfMeasureValue*sizeof(int) );
     MeasureOffset       =(int *)malloc( countOfMeasureValue*sizeof(int) );
     MeasureByteSequence =(int *)malloc( countOfMeasureValue*sizeof(int) );
     MeasureValue        =(float *)malloc( countOfMeasureValue*sizeof(float) );
     MeasureValueOld     =(float *)malloc( countOfMeasureValue*sizeof(float) );
     MeasureValueNew     =(float *)malloc( countOfMeasureValue*sizeof(float) );
     MeasureValueKoef    =(float *)malloc( countOfMeasureValue*sizeof(float) );
     ii=0;
     for (i = 0; i < json_array_get_count(jArray); i++)
    {
        jObj = json_array_get_object(jArray, i);
        if(json_object_dotget_string(jObj, "MeasureValue.VarName")!=NULL)
        {
        printf("%s:%s:%s:%s:%s\n",
               json_object_dotget_string(jObj, "MeasureValue.VarName"),
               json_object_dotget_string(jObj, "MeasureValue.IecAddress"),
               json_object_dotget_string(jObj, "MeasureValue.OffSet"),
               json_object_dotget_string(jObj, "MeasureValue.ByteCount"),
               json_object_dotget_string(jObj, "MeasureValue.ByteSequence"));


         MeasureVarname[ii]     =json_object_dotget_string(jObj, "MeasureValue.VarName");
         MeasureAddress[ii]     =atoi(json_object_dotget_string(jObj, "MeasureValue.IecAddress"));
         MeasureOffset[ii]      =atoi(json_object_dotget_string(jObj, "MeasureValue.OffSet"));
         MeasureByteCount[ii]   =atoi(json_object_dotget_string(jObj, "MeasureValue.ByteCount"));
         MeasureByteSequence[ii]=atoi(json_object_dotget_string(jObj, "MeasureValue.ByteSequence"));
         MeasureValueOld[ii]    =0;
         MeasureValueNew[ii]    =0;           
         MeasureValue[ii]       =0;
         MeasureVarType[ii]     =json_object_dotget_string(jObj, "MeasureValue.VarType");  
         MeasureValueKoef[ii]   =atof(json_object_dotget_string(jObj, "MeasureValue.Koef"));    
         ii=ii+1;
        }
    }

}




void sigint_handler(int signalId)
{
    running = false;
}


static bool interrogationHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu, uint8_t qoi)
{
     int idi;
     printf("Received interrogation for group %i\n", qoi);

     if (qoi == 20)
     {

         CS101_AppLayerParameters alParams = IMasterConnection_getApplicationLayerParameters(connection);
         IMasterConnection_sendACT_CON(connection, asdu, false);
         
//-----------------------------------------for float----------------------------------------------------------------------------------------         
         CS101_ASDU newAsduFloat = CS101_ASDU_create(alParams, false, CS101_COT_INTERROGATED_BY_STATION,0, 1, false, false);
         InformationObject ioFloat = (InformationObject) MeasuredValueShort_create(NULL, 0,0, IEC60870_QUALITY_GOOD);
         for (idi=0; idi<countOfMeasureValue; idi++)
        {        
             if(strcoll (MeasureVarType[idi],"float") == 0)
            {
             CS101_ASDU_addInformationObject(newAsduFloat, (InformationObject) MeasuredValueShort_create((MeasuredValueShort) ioFloat, MeasureAddress[idi], MeasureValue[idi], IEC60870_QUALITY_GOOD));
            }         
        }
         InformationObject_destroy(ioFloat);
         IMasterConnection_sendASDU(connection, newAsduFloat);
         CS101_ASDU_destroy(newAsduFloat);

//-----------------------------------------for int32----------------------------------------------------------------------------------------         
         CS101_ASDU newAsduInt32 = CS101_ASDU_create(alParams, false, CS101_COT_INTERROGATED_BY_STATION,0, 1, false, false);
         InformationObject ioInt32 = (InformationObject) MeasuredValueShort_create(NULL, 0,0, IEC60870_QUALITY_GOOD);
         for (idi=0; idi<countOfMeasureValue; idi++)
        {        
             if(strcoll (MeasureVarType[idi],"int32") == 0)
            {
             CS101_ASDU_addInformationObject(newAsduInt32, (InformationObject) MeasuredValueShort_create((MeasuredValueShort) ioInt32, MeasureAddress[idi], MeasureValue[idi], IEC60870_QUALITY_GOOD));
            }         
        }
         InformationObject_destroy(ioInt32);
         IMasterConnection_sendASDU(connection, newAsduInt32);
         CS101_ASDU_destroy(newAsduInt32);
//----------------------------------------for int-------------------------------------------------------------------
         CS101_ASDU newAsduInt = CS101_ASDU_create(alParams, false, CS101_COT_INTERROGATED_BY_STATION,0, 1, false, false);
         InformationObject ioInt = (InformationObject) MeasuredValueScaled_create(NULL, 0,0, IEC60870_QUALITY_GOOD);
         for (idi=0; idi<countOfMeasureValue; idi++)
        {
             if(strcoll (MeasureVarType[idi],"int") == 0)
            {
             CS101_ASDU_addInformationObject(newAsduInt, (InformationObject) MeasuredValueScaled_create((MeasuredValueScaled) ioInt, MeasureAddress[idi], MeasureValue[idi], IEC60870_QUALITY_GOOD));
            }
        }
         InformationObject_destroy(ioInt);
         IMasterConnection_sendASDU(connection, newAsduInt);
         CS101_ASDU_destroy(newAsduInt);   

//-------------------------------------------for bool-----------------------------------------------------------------------------
         CS101_ASDU newAsduBool = CS101_ASDU_create(alParams, false, CS101_COT_INTERROGATED_BY_STATION,0, 1, false, false);
         InformationObject ioBool = (InformationObject) SinglePointInformation_create(NULL, 0,0, IEC60870_QUALITY_GOOD);
         for (idi=0; idi<countOfSinglePoint; idi++)
        {
         CS101_ASDU_addInformationObject(newAsduBool, (InformationObject) SinglePointInformation_create((SinglePointInformation) ioBool,SinglePointAddress[idi], SinglePointValue[idi], IEC60870_QUALITY_GOOD));
        }
         InformationObject_destroy(ioBool);
         IMasterConnection_sendASDU(connection, newAsduBool);
         CS101_ASDU_destroy(newAsduBool);
         IMasterConnection_sendACT_TERM(connection, asdu);
     }
     else
     {
        IMasterConnection_sendACT_CON(connection, asdu, true);
     }

    return true;
}


static bool connectionRequestHandler(void* parameter, const char* ipAddress)
{
    printf("New connection from %s\n", ipAddress);

#if 0
    if (strcmp(ipAddress, "127.0.0.1") == 0) {
        printf("Accept connection\n");
        return true;
    }
    else {
        printf("Deny connection\n");
        return false;
    }
#else
    return true;
#endif
}

static bool asduHandler(void* parameter, IMasterConnection connection, CS101_ASDU asdu)
{
     if (CS101_ASDU_getTypeID(asdu) == C_SC_NA_1)
    {
         printf("received single command\n");
         if  (CS101_ASDU_getCOT(asdu) == CS101_COT_ACTIVATION)
        {
             InformationObject io = CS101_ASDU_getElement(asdu, 0);
             if (InformationObject_getObjectAddress(io) == 5000)
            {
                 SingleCommand sc = (SingleCommand) io;
                 printf("IOA: %i switch to %i\n", InformationObject_getObjectAddress(io),
                 SingleCommand_getState(sc));
                 CS101_ASDU_setCOT(asdu, CS101_COT_ACTIVATION_CON);
            }
             else
            {
                CS101_ASDU_setCOT(asdu, CS101_COT_UNKNOWN_IOA);
            }
             InformationObject_destroy(io);
        }
         else
        {
            CS101_ASDU_setCOT(asdu, CS101_COT_UNKNOWN_COT);
        }
        IMasterConnection_sendASDU(connection, asdu);
        return true;
    }
     return false;
}

void sendDataMeasuredValueFloat(void* alParams,int adr,float val,CS104_Slave slave)
{
     CS101_ASDU newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);
     //InformationObject io =  (InformationObject) MeasuredValueScaled_create(NULL, adr, val, IEC60870_QUALITY_GOOD);
     InformationObject io =  (InformationObject) MeasuredValueShort_create(NULL, adr, val, IEC60870_QUALITY_GOOD);
     
     CS101_ASDU_addInformationObject(newAsdu, io);
     InformationObject_destroy(io);
     CS104_Slave_enqueueASDU(slave, newAsdu);
     CS101_ASDU_destroy(newAsdu);
}


void sendDataMeasuredValueInt(void* alParams,int adr,int val,CS104_Slave slave)
{
     CS101_ASDU newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);
     InformationObject io =  (InformationObject) MeasuredValueScaled_create(NULL, adr, val, IEC60870_QUALITY_GOOD);
     CS101_ASDU_addInformationObject(newAsdu, io);
     InformationObject_destroy(io);
     CS104_Slave_enqueueASDU(slave, newAsdu);
     CS101_ASDU_destroy(newAsdu);
}



void sendDataSinglePoint(void* alParams,int adr,bool val,CS104_Slave slave)
{
     CS101_ASDU newAsdu = CS101_ASDU_create(alParams, false, CS101_COT_PERIODIC, 0, 1, false, false);
    // newAsdu = CS101_ASDU_create(alParams, true, CS101_COT_INTERROGATED_BY_STATION,0, 1, false, false);
     InformationObject io = (InformationObject) SinglePointInformation_create(NULL,adr,val, IEC60870_QUALITY_GOOD);
     CS101_ASDU_addInformationObject(newAsdu, io);
     InformationObject_destroy(io);
     CS104_Slave_enqueueASDU(slave, newAsdu);
     CS101_ASDU_destroy(newAsdu);
}
int main(int argc, char **argv)
{
     if(argc <= 1) {
         printf("\nUse syntax : server104.exe file.json port ip\n");
         return 0;
     }
    
    
     //  udp data
     int clilen, n,i,z,p;
     char line[1000];
     int sockfd;
     struct sockaddr_in servaddr, cliaddr;
     char jline[1000];
     int boffset[1000];
     printf("-- Start programm v.2.05 --\n");
     bzero(&servaddr, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     //servaddr.sin_port = htons(64002);
     servaddr.sin_port = htons(atoi(argv[2]));
     
     servaddr.sin_addr.s_addr = htonl(INADDR_ANY);



     printf("-- Init UDP --\n");
      if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
    {   printf("-- Init UDP server Error --\n");
        perror(NULL);
        exit(1);
    }
     if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
    {
      printf("-- Bind Socket Error --\n");
      perror(NULL);
      close(sockfd);
      exit(1);
    }

     // json data
     JSON_Value *val;
     JSON_Array *array;
     JSON_Value *root_value;
     JSON_Object *jObj;
     char *istr;
     char ss[100];
         union
        {
             int inValue;
             float floatValue;
        } unionFloat;
        
    // val = json_parse_file("config.json");
   //  json_parse_string(json_serialize_to_string(val));
     printf("-- Starting server UDP     -- \n");
     printf("-- Starting server iec 104 -- \n");
     LoadJsonFile(argv[1]);
   //  initVariables();
     signal(SIGINT, sigint_handler);
     CS104_Slave slave = CS104_Slave_create(100, 100);
     CS104_Slave_setLocalAddress(slave, argv[3]);
     CS104_Slave_setServerMode(slave, CS104_MODE_SINGLE_REDUNDANCY_GROUP);
     CS101_AppLayerParameters alParams = CS104_Slave_getAppLayerParameters(slave);
     CS104_Slave_setInterrogationHandler(slave, interrogationHandler, NULL);
     CS104_Slave_setASDUHandler(slave, asduHandler, NULL);

     CS104_Slave_setConnectionRequestHandler(slave, connectionRequestHandler, NULL);
     CS104_Slave_start(slave);

     if (CS104_Slave_isRunning(slave) == false)
     {
        printf("Starting server failed!\n");
        goto exit_program;
     }

     //int16_t scaledValue = 0;

     while (running)
    {

         Thread_sleep(1000);
         memset(line, 0, 999);
         clilen = sizeof(cliaddr);
         if((n = recvfrom(sockfd, line, 999, 0, (struct sockaddr *) &cliaddr, &clilen)) < 0)
        {
             perror(NULL);
             close(sockfd);
             exit(1);
        }
//--------------get input line & parse string
         printf("--> %s\n", line);
         for(i=0; i<strlen(line); i++)
        {
              if(line[i] == '[') line[i]='\"';
              if(line[i] == ']') line[i]='\"';
        }
         strcpy(jline, "[");
         strcat(jline, line);
         strcat(jline, "]");
         //printf("-------------%s\n",jline);
      //   json_array_clear(array);
         array=json_value_get_array(json_parse_string(jline));
         jObj = json_array_get_object(array, 0);
         if(json_object_dotget_string(jObj, "name")!=NULL)
        {
//-------------single  Point Data-------------------------------------------------------------------------------------------------------------------
             for(i=0; i<countOfSinglePoint; i++)
            {                       
                 if(strcoll (json_object_dotget_string(jObj, "name"), SinglePointVarname[i] ) == 0)
                {
            //printf("%s == %s : data -- %s\n",SinglePointVarname[i],json_object_dotget_string(jObj, "name"),json_object_dotget_string(jObj, "data") );
                     strcpy(ss, json_object_dotget_string(jObj, "data"));
                     istr = strtok (ss,",");
                     z=0;
                     while (istr != NULL)
                    {
                        // printf ("reg:%d=%d |iec adr: %d offset %d\n",z,atoi(istr),SinglePointAddress[i],SinglePointOffset[z]);
                         if(SinglePointOffset[i] == z)
                        {
                             printf ("Set ofset:%d value %d iec adr: %d offset %d\n",z,atoi(istr),SinglePointAddress[i],SinglePointOffset[i]);
         //                    sendDataMeasuredValue(alParams,SinglePointAddress[i],atoi(istr),slave);
                             if(atoi(istr)==0)
                            {    
                             SinglePointValue[i]=false;
                             SinglePointNewData[i]=false; 
                            }
                             else
                            {    
                             SinglePointValue[i]  =true;
                             SinglePointNewData[i]=true;
                            }
                             
                             if(SinglePointOldData[i] != SinglePointValue[i])
                            {
                                 sendDataSinglePoint(alParams,SinglePointAddress[i],SinglePointValue[i],slave);
                                 SinglePointOldData[i]=SinglePointNewData[i];
                            }    
                        }
                         istr = strtok (NULL,",");
                         z++;
                    }
                }
                 else
                {
                }                    
            }        
//----------------------------mesure Value                        
             for(i=0; i<countOfMeasureValue; i++)
            {                       
                 if(strcoll (json_object_dotget_string(jObj, "name"), MeasureVarname[i] ) == 0)
                {
                    //----------------------------int----------------------------------------------                     
                     if(strcoll (MeasureVarType[i],"int") == 0)   
                    {    
                         strcpy(ss, json_object_dotget_string(jObj, "data"));
                         istr = strtok (ss,",");
                         z=0;
                         while (istr != NULL)
                        {
                             boffset[z]=atoi(istr);
                             istr = strtok (NULL,",");
                             z++;
                        }  
                         for(p=0; p<z; p++) 
                        {
                             if(MeasureOffset[i] == p)
                            {
                                 MeasureValue[i]=boffset[p];
                                 MeasureValue[i]=MeasureValue[i]*MeasureValueKoef[i];
                                 if(MeasureValueOld[i] != MeasureValue[i])
                                {
                                     sendDataMeasuredValueInt(alParams,MeasureAddress[i],MeasureValue[i],slave);
                                     MeasureValueOld[i]=MeasureValue[i];
                                }    
                            }
                        }
                    } 
             //----------------------------int32------------------------------------------------------------------------                     
                     if(strcoll (MeasureVarType[i],"int32") == 0)   
                    {    
                         strcpy(ss, json_object_dotget_string(jObj, "data"));
                         istr = strtok (ss,",");
                         z=0; 
                         while (istr != NULL)
                        {
                             boffset[z]=atoi(istr);
                             istr = strtok (NULL,",");
                             z++;
                        }  
                         for(p=0; p<z; p++) 
                        {
                            if(MeasureOffset[i] == p)
                            {
                                 if(MeasureByteCount[i]==2)
                                {
                                     if(MeasureByteSequence[i]==21)
                                    {    
                                         MeasureValue[i]= (boffset[p+1]<<16) + boffset[p];
                                         MeasureValue[i]=MeasureValue[i]*MeasureValueKoef[i];
                                    }
                                    else
                                    {
                                         MeasureValue[i]= (boffset[p]<<16) + boffset[p+1];
                                         MeasureValue[i]=MeasureValue[i]*MeasureValueKoef[i];
                                    }
                                }
                                 else
                                {
                                 MeasureValue[i]=boffset[p];
                                 MeasureValue[i]=MeasureValue[i]*MeasureValueKoef[i];
                                }                                 
                                 if(MeasureValueOld[i] != MeasureValue[i])
                                {
                                     sendDataMeasuredValueFloat(alParams,MeasureAddress[i],MeasureValue[i],slave);
                                     MeasureValueOld[i]=MeasureValue[i];
                                }    
                            }
                        }
                    }
                    //----------------------------float------------------------------------------------------------------------                     
                     if(strcoll (MeasureVarType[i],"float") == 0)   
                    {    
                         strcpy(ss, json_object_dotget_string(jObj, "data"));
                         istr = strtok (ss,",");
                         z=0; 
                         while (istr != NULL)
                        {
                             boffset[z]=atoi(istr);
                             istr = strtok (NULL,",");
                             z++;
                        }  
                         for(p=0; p<z; p++) 
                        {
                            if(MeasureOffset[i] == p)
                            {
                                 if(MeasureByteCount[i]==2)
                                {
                                     if(MeasureByteSequence[i]==21)
                                    {    
                                         unionFloat.inValue = (boffset[p+1]<<16) + boffset[p]; 
                                         MeasureValue[i]=unionFloat.floatValue*MeasureValueKoef[i];
                                    }
                                    else
                                    {
                                         unionFloat.inValue = (boffset[p]<<16) + boffset[p+1]; 
                                         MeasureValue[i]=unionFloat.floatValue*MeasureValueKoef[i];
                                    }                                   
                                }
                                 else
                                {
                                 MeasureValue[i]=boffset[p];
                                 MeasureValue[i]=MeasureValue[i]*MeasureValueKoef[i];
                                }                                 
                                 if(MeasureValueOld[i] != MeasureValue[i])
                                {
                                     sendDataMeasuredValueFloat(alParams,MeasureAddress[i],MeasureValue[i],slave);
                                     MeasureValueOld[i]=MeasureValue[i];
                                }    
                            }
                        }
                    }
                    //----------------------------------------------------------------------------------------------------------                     
                }   
                 else
                {
                }
            }
           json_object_clear(jObj);
        }
    }

     CS104_Slave_stop(slave);

exit_program:
    CS104_Slave_destroy(slave);
    Thread_sleep(500);
}

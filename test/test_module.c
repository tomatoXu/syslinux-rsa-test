#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "crypto.h"
#define CONFIG_SSL_FULL_MODE

void main(){

        FILE *fp;
        fp = fopen("in_data.dat","r");
        if(fp == NULL){
                printf("errer on open!\n");
                exit(1);
        }
        int pub_num = 0;
        fscanf(fp,"%d",&pub_num);
        while(!feof(fp)){
                //int pub_num = 0;
                int priv_num = 0;
                int m_num = 0;
                int test = 0;
                int data_num = 0;
               // fscanf(fp,"%d",&pub_num);
                fscanf(fp,"%d",&priv_num);
                fscanf(fp,"%d",&m_num);
                fscanf(fp,"%d",&data_num);
                uint16_t i = (unsigned int)data_num;
                uint8_t pub[64];
                uint8_t priv[64];
                uint8_t m[64];
                uint8_t a[64];
                int l = 0;
                uint8_t data;
                while(l<pub_num+priv_num+m_num+data_num){
                        if(l < pub_num){
                                fscanf(fp,"%02x",&pub[l]);
                                l++;
                }
                        else if(l < pub_num+priv_num&&l>=pub_num){
                                fscanf(fp,"%02x",&priv[l-pub_num]);
                                l++;
                }
                        else if(l<pub_num+priv_num+m_num){
                                fscanf(fp,"%02x",&m[l-priv_num-pub_num]);
                                l++;
                }
                        else{
                                fscanf(fp,"%02x",&a[l-priv_num-pub_num-m_num]);
                                l++;
                        }
                }
        


                RSA_CTX *ctx;
                RSA_CTX **temp;
                temp = &ctx;
                uint8_t *pub_exp;
                uint8_t *priv_exp;
                uint8_t *modulus;
                uint8_t *in_data;
                pub_exp = pub;
                priv_exp = priv;
                modulus = m;
                in_data = a;

                RSA_priv_key_new(temp,modulus,m_num,pub_exp,pub_num,priv_exp,priv_num);
                RSA_print(ctx);
                printf("message:");
                int q = 0;
                for(;q<data_num;q++){
                        printf("0x%02x ",a[q]);
                }
                printf("\n\n");
                uint8_t *out_data = (unsigned char*)malloc(32);
                int byte_size = RSA_encrypt(ctx,in_data,i,out_data,0);

                uint8_t *message = (unsigned char*)malloc(ctx->num_octets);
                int dec = RSA_decrypt(ctx,out_data,message,1);
                printf("success:%d\n",dec);
                printf("message:");
                int k = 0;
                for (;k<data_num;k++){
                        printf("0x%02x ",message[k]);
                }
                printf("\n");
                fscanf(fp,"%02x",&pub_num);
                }
        fclose(fp);

}

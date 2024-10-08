#include <ModbusMaster.h>

#define BLYNK_TEMPLATE_ID "TMPL6GnkniVSv"
#define BLYNK_DEVICE_NAME "Elora"
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_ESP32S2_DEV_KIT

#include "BlynkEdgent.h"

static uint16_t SCHNEIDER = 0x01;

ModbusMaster node1;

int milis=200; //delay variable
//uint16_t cmnd; double cek1,cek2,cek3,cek4;

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1);

  node1.begin(SCHNEIDER, Serial2);
  //node1.preTransmission(preTransmission);
  //node1.postTransmission(postTransmission);
  BlynkEdgent.begin();
}

//2 ARRAY TO FLOAT======================================================================================================================
float volt_an, volt_bn, volt_cn, volt_lnavg;
float volt_ab, volt_bc, volt_ca, volt_llavg;
float unbal_an, unbal_bn, unbal_cn, unbal_lnwrst;
float unbal_ab, unbal_bc, unbal_ca, unbal_llwrst;
float actpo_a, actpo_b, actpo_c, actpo_tot;
float reacpo_a, reacpo_b, reacpo_c, reacpo_tot;
float appo_a, appo_b, appo_c, appo_tot;
float pf_a, pf_b, pf_c, pf_tot;
float cthd_ll, cthd_ln, cthd_a, cthd_b, cthd_c, cthd_n, cthd_g;
float vthd_an, vthd_bn, vthd_cn, vthd_ln,vthd_ab, vthd_bc, vthd_ca, vthd_ll;
float minv_ab,minv_bc,minv_ca,minv_ll,minv_an,minv_bn,minv_cn,minv_ln;
float maxv_ab,maxv_bc,maxv_ca,maxv_ll,maxv_an,maxv_bn,maxv_cn,maxv_ln;
float amp_a, amp_b, amp_c, amp_n, amp_g, amp_avg;
float cunbal_a, cunbal_b, cunbal_c, cunbal_wrst;
int8_t rst,rst1;

unsigned int  van[2], vbn[2], vcn[2], vln[2];
unsigned int  vab[2], vbc[2], vca[2], vll[2];
unsigned int  uban[2], ubbn[2], ubcn[2], ubln[2];
unsigned int  ubab[2], ubbc[2], ubca[2], ubll[2];
unsigned int  acpoa[2], acpob[2], acpoc[2], acpot[2];
unsigned int  repoa[2], repob[2], repoc[2], repot[2];
unsigned int  apoa[2], apob[2], apoc[2], apot[2];
unsigned int  pfa[2], pfb[2], pfc[2], pft[2];
unsigned int  cthdll[2], cthdln[2], cthda[2], cthdb[2], cthdc[2], cthdn[2], cthdg[2];
unsigned int  vthdan[2], vthdbn[2], vthdcn[2], vthdln[2], vthdab[2], vthdbc[2], vthdca[2], vthdll[2];
unsigned int  minvab[2], minvbc[2], minvca[2], minvll[2], minvan[2], minvbn[2], minvcn[2], minvln[2];
unsigned int  maxvab[2], maxvbc[2], maxvca[2], maxvll[2], maxvan[2], maxvbn[2], maxvcn[2], maxvln[2];
unsigned int  aa[2], ab[2], ac[2], an[2], ag[2], aavg[2];
unsigned int  uba[2], ubb[2], ubc[2], ubw[2];

int16_t data1,data2,data3,data4,data5,data6,data7,data8; //VOLT
int16_t data9,data10,data11,data12,data13,data14,data15,data16; //VOLT UNBALANCE
int16_t data17,data18,data19,data20,data21,data22,data23,data24,data25,data26,data27,data28; //POWER
int32_t data29,data30,data31,data32; //PF
int16_t data39,data40,data41,data42,data43,data44,data45; //CURRENT THD
int16_t data46,data47,data48,data49,data50,data51,data52,data53; //VOLTAGE THD
int16_t data54,data55,data56,data57,data58,data59,data60,data61; //MINIMUM VALUES
int16_t data62,data63,data64,data65,data66,data67,data68,data69; //MAXIMUM VALUES
int16_t data70,data71,data72,data73,data74,data75; //CURRENT
int16_t data76,data77,data78,data79; //CURRENT UNBALANCE
uint16_t data80,data81,data82,data83,data84;
uint16_t reset_energy,reset_energy1;

float f_2uint_float(unsigned int uint1, unsigned int uint2) {    // reconstruct the float from 2 unsigned integers

  union f_2uint {
    float f;
    uint16_t i[3];
  };

  union f_2uint f_number;
  f_number.i[0] = uint1;
  f_number.i[1] = uint2;

  return f_number.f;
}
//END OF 2 ARRAY TO FLOAT======================================================================================================================

//4 ARRAY TO FLOAT======================================================================================================================
int64_t aed, aer, red, rer, aped, aper;

unsigned long  aedd[4], aerr[4], redd[4], rerr[4], apedd[4], aperr[4];

int64_t data33,data34,data35,data36,data37,data38; //ENERGY

int64_t l_4uint_int64(unsigned int uint1, unsigned int uint2, unsigned int uint3, unsigned int uint4) {    // reconstruct the float from 4 unsigned integers

  union l_4uint {
    long l;
    uint16_t i[5];
  };

  union l_4uint l_number;
  l_number.i[0] = uint1;
  l_number.i[1] = uint2;
  l_number.i[2] = uint3;
  l_number.i[3] = uint4;

  return l_number.l;
}
//END OF 4 ARRAY TO FLOAT======================================================================================================================

void VSCHNEIDER()
{
//VOLTAGE===========================================================================================================================
  data1 = node1.readHoldingRegisters(3027, 2);
  if (data1 == node1.ku8MBSuccess) 
  {
    van[0] = node1.getResponseBuffer(0x00);
    van[1] = node1.getResponseBuffer(0x01);
    volt_an = f_2uint_float(van[1],van[0]);

    Blynk.virtualWrite(V3, volt_an); 
    delay(milis);
  }
  data2 = node1.readHoldingRegisters(3029, 2);
  if (data2 == node1.ku8MBSuccess) 
  {
    vbn[0] = node1.getResponseBuffer(0x00);
    vbn[1] = node1.getResponseBuffer(0x01);
    volt_bn = f_2uint_float(vbn[1],vbn[0]);

    Blynk.virtualWrite(V4, volt_bn); 
    delay(milis);
  }
  data3 = node1.readHoldingRegisters(3031, 2);
  if (data3 == node1.ku8MBSuccess) 
  {
    vcn[0] = node1.getResponseBuffer(0x00);
    vcn[1] = node1.getResponseBuffer(0x01);
    volt_cn = f_2uint_float(vcn[1],vcn[0]);

    Blynk.virtualWrite(V5, volt_cn); 
    delay(milis);
  }
  data4 = node1.readHoldingRegisters(3035, 2);
  if (data4 == node1.ku8MBSuccess) 
  {
    vln[0] = node1.getResponseBuffer(0x00);
    vln[1] = node1.getResponseBuffer(0x01);
    volt_lnavg = f_2uint_float(vln[1],vln[0]);

    Blynk.virtualWrite(V6, volt_lnavg); 
    delay(milis);
  }
  data5 = node1.readHoldingRegisters(3019, 2);
  if (data5 == node1.ku8MBSuccess) 
  {
    vab[0] = node1.getResponseBuffer(0x00);
    vab[1] = node1.getResponseBuffer(0x01);
    volt_ab = f_2uint_float(vab[1],vab[0]);

    Blynk.virtualWrite(V7, volt_ab); 
    delay(milis);
  }
  data6 = node1.readHoldingRegisters(3021, 2);
  if (data6 == node1.ku8MBSuccess) 
  {
    vbc[0] = node1.getResponseBuffer(0x00);
    vbc[1] = node1.getResponseBuffer(0x01);
    volt_bc = f_2uint_float(vbc[1],vbc[0]);

    Blynk.virtualWrite(V8, volt_bc); 
    delay(milis);
  }
  data7 = node1.readHoldingRegisters(3023, 2);
  if (data7 == node1.ku8MBSuccess) 
  {
    vca[0] = node1.getResponseBuffer(0x00);
    vca[1] = node1.getResponseBuffer(0x01);
    volt_ca = f_2uint_float(vca[1],vca[0]);

    Blynk.virtualWrite(V9, volt_ca); 
    delay(milis);
  }
  data8 = node1.readHoldingRegisters(3025, 2);
  if (data8 == node1.ku8MBSuccess) 
  {
    vll[0] = node1.getResponseBuffer(0x00);
    vll[1] = node1.getResponseBuffer(0x01);
    volt_llavg = f_2uint_float(vll[1],vll[0]);

    Blynk.virtualWrite(V10, volt_lnavg); 
    delay(milis);
  }
//END OF VOLTAGE===========================================================================================================================

//VOLTAGE UNBALANCE===========================================================================================================================
  data9 = node1.readHoldingRegisters(3045, 2);
  if (data9 == node1.ku8MBSuccess) 
  {
    uban[0] = node1.getResponseBuffer(0x00);
    uban[1] = node1.getResponseBuffer(0x01);
    unbal_an = f_2uint_float(uban[1],uban[0]);

    Blynk.virtualWrite(V11, unbal_an); 
    delay(milis);
  }
  data10 = node1.readHoldingRegisters(3047, 2);
  if (data10 == node1.ku8MBSuccess) 
  {
    ubbn[0] = node1.getResponseBuffer(0x00);
    ubbn[1] = node1.getResponseBuffer(0x01);
    unbal_bn = f_2uint_float(ubbn[1],ubbn[0]);

    Blynk.virtualWrite(V12, unbal_bn); 
    delay(milis);
  }
  data11 = node1.readHoldingRegisters(3049, 2);
  if (data11 == node1.ku8MBSuccess) 
  {
    ubcn[0] = node1.getResponseBuffer(0x00);
    ubcn[1] = node1.getResponseBuffer(0x01);
    unbal_cn = f_2uint_float(ubcn[1],ubcn[0]);

    Blynk.virtualWrite(V13, unbal_cn); 
    delay(milis);
  }
  data12 = node1.readHoldingRegisters(3051, 2);
  if (data12 == node1.ku8MBSuccess) 
  {
    ubln[0] = node1.getResponseBuffer(0x00);
    ubln[1] = node1.getResponseBuffer(0x01);
    unbal_lnwrst = f_2uint_float(ubln[1],ubln[0]);

    Blynk.virtualWrite(V14, unbal_lnwrst); 
    delay(milis);
  }
  data13 = node1.readHoldingRegisters(3037, 2);
  if (data13 == node1.ku8MBSuccess) 
  {
    ubab[0] = node1.getResponseBuffer(0x00);
    ubab[1] = node1.getResponseBuffer(0x01);
    unbal_ab = f_2uint_float(ubab[1],ubab[0]);

    Blynk.virtualWrite(V15, unbal_ab); 
    delay(milis);
  }
  data14 = node1.readHoldingRegisters(3039, 2);
  if (data14 == node1.ku8MBSuccess) 
  {
    ubbc[0] = node1.getResponseBuffer(0x00);
    ubbc[1] = node1.getResponseBuffer(0x01);
    unbal_bc = f_2uint_float(ubbc[1],ubbc[0]);

    Blynk.virtualWrite(V16, unbal_bc); 
    delay(milis);
  }
  data15 = node1.readHoldingRegisters(3041, 2);
  if (data15 == node1.ku8MBSuccess) 
  {
    ubca[0] = node1.getResponseBuffer(0x00);
    ubca[1] = node1.getResponseBuffer(0x01);
    unbal_ca = f_2uint_float(ubca[1],ubca[0]);

    Blynk.virtualWrite(V17, unbal_ca); 
    delay(milis);
  }
  data16 = node1.readHoldingRegisters(3043, 2);
  if (data16 == node1.ku8MBSuccess) 
  {
    ubll[0] = node1.getResponseBuffer(0x00);
    ubll[1] = node1.getResponseBuffer(0x01);
    unbal_llwrst = f_2uint_float(ubll[1],ubll[0]);

    Blynk.virtualWrite(V18, unbal_lnwrst); 
    delay(milis);
  }
//END OF VOLTAGE UNBALANCE===========================================================================================================================

//POWER===========================================================================================================================
//=========ACTIVE POWER==========
  data17 = node1.readHoldingRegisters(3053, 2);
  if (data17 == node1.ku8MBSuccess) 
  {
    acpoa[0] = node1.getResponseBuffer(0x00);
    acpoa[1] = node1.getResponseBuffer(0x01);
    actpo_a = f_2uint_float(acpoa[1],acpoa[0]);

    Blynk.virtualWrite(V19, actpo_a); 
    delay(milis);
  }
  data18 = node1.readHoldingRegisters(3055, 2);
  if (data18 == node1.ku8MBSuccess) 
  {
    acpob[0] = node1.getResponseBuffer(0x00);
    acpob[1] = node1.getResponseBuffer(0x01);
    actpo_b = f_2uint_float(acpob[1],acpob[0]);

    Blynk.virtualWrite(V20, actpo_b); 
    delay(milis);
  }
  data19 = node1.readHoldingRegisters(3057, 2);
  if (data19 == node1.ku8MBSuccess) 
  {
    acpoc[0] = node1.getResponseBuffer(0x00);
    acpoc[1] = node1.getResponseBuffer(0x01);
    actpo_c = f_2uint_float(acpoc[1],acpoc[0]);

    Blynk.virtualWrite(V21, actpo_c); 
    delay(milis);
  }
  data20 = node1.readHoldingRegisters(3059, 2);
  if (data20 == node1.ku8MBSuccess) 
  {
    acpot[0] = node1.getResponseBuffer(0x00);
    acpot[1] = node1.getResponseBuffer(0x01);
    actpo_tot = f_2uint_float(acpot[1],acpot[0]);

    Blynk.virtualWrite(V22, actpo_tot); 
    delay(milis);
  }
//=========END OF ACTIVE POWER==========

//=========REACTIVE POWER==========
  data21 = node1.readHoldingRegisters(3061, 2);
  if (data21 == node1.ku8MBSuccess) 
  {
    repoa[0] = node1.getResponseBuffer(0x00);
    repoa[1] = node1.getResponseBuffer(0x01);
    reacpo_a = f_2uint_float(repoa[1],repoa[0]);

    Blynk.virtualWrite(V23, reacpo_a); 
    delay(milis);
  }
  data22 = node1.readHoldingRegisters(3063, 2);
  if (data22 == node1.ku8MBSuccess) 
  {
    repob[0] = node1.getResponseBuffer(0x00);
    repob[1] = node1.getResponseBuffer(0x01);
    reacpo_b = f_2uint_float(repob[1],repob[0]);

    Blynk.virtualWrite(V24, reacpo_b); 
    delay(milis);
  }
  data23 = node1.readHoldingRegisters(3065, 2);
  if (data23 == node1.ku8MBSuccess) 
  {
    repoc[0] = node1.getResponseBuffer(0x00);
    repoc[1] = node1.getResponseBuffer(0x01);
    reacpo_c = f_2uint_float(repoc[1],repoc[0]);

    Blynk.virtualWrite(V25, reacpo_c); 
    delay(milis);
  }
  data24 = node1.readHoldingRegisters(3067, 2);
  if (data24 == node1.ku8MBSuccess) 
  {
    repot[0] = node1.getResponseBuffer(0x00);
    repot[1] = node1.getResponseBuffer(0x01);
    reacpo_tot = f_2uint_float(repot[1],repot[0]);

    Blynk.virtualWrite(V26, reacpo_tot); 
    delay(milis);
  }
//=========END OF REACTIVE POWER==========

//=========APPARENT POWER==========
  data25 = node1.readHoldingRegisters(3069, 2);
  if (data25 == node1.ku8MBSuccess) 
  {
    apoa[0] = node1.getResponseBuffer(0x00);
    apoa[1] = node1.getResponseBuffer(0x01);
    appo_a = f_2uint_float(apoa[1],apoa[0]);

    Blynk.virtualWrite(V27, appo_a); 
    delay(milis);
  }
  data26 = node1.readHoldingRegisters(3071, 2);
  if (data26 == node1.ku8MBSuccess) 
  {
    apob[0] = node1.getResponseBuffer(0x00);
    apob[1] = node1.getResponseBuffer(0x01);
    appo_b = f_2uint_float(apob[1],apob[0]);

    Blynk.virtualWrite(V28, appo_b); 
    delay(milis);
  }
  data27 = node1.readHoldingRegisters(3073, 2);
  if (data27 == node1.ku8MBSuccess) 
  {
    apoc[0] = node1.getResponseBuffer(0x00);
    apoc[1] = node1.getResponseBuffer(0x01);
    appo_c = f_2uint_float(apoc[1],apoc[0]);

    Blynk.virtualWrite(V29, appo_c); 
    delay(milis);
  }
  data28 = node1.readHoldingRegisters(3075, 2);
  if (data28 == node1.ku8MBSuccess) 
  {
    apot[0] = node1.getResponseBuffer(0x00);
    apot[1] = node1.getResponseBuffer(0x01);
    appo_tot = f_2uint_float(apot[1],apot[0]);

    Blynk.virtualWrite(V30, appo_tot); 
    delay(milis);
  }
//=========END OF APPARENT POWER==========
//END OF POWER===========================================================================================================================

//PF===========================================================================================================================
  data29 = node1.readHoldingRegisters(3077, 2);
  if (data29 == node1.ku8MBSuccess) 
  {
    pfa[0] = node1.getResponseBuffer(0x00);
    pfa[1] = node1.getResponseBuffer(0x01);
    pf_a = f_2uint_float(pfa[1],pfa[0]);

    Blynk.virtualWrite(V31, pf_a); 
    delay(milis);
  }
  data30 = node1.readHoldingRegisters(3079, 2);
  if (data30 == node1.ku8MBSuccess) 
  {
    pfb[0] = node1.getResponseBuffer(0x00);
    pfb[1] = node1.getResponseBuffer(0x01);
    pf_b = f_2uint_float(pfb[1],pfb[0]);

    Blynk.virtualWrite(V32, pf_b); 
    delay(milis);
  }
  data31 = node1.readHoldingRegisters(3081, 2);
  if (data31 == node1.ku8MBSuccess) 
  {
    pfc[0] = node1.getResponseBuffer(0x00);
    pfc[1] = node1.getResponseBuffer(0x01);
    pf_c = f_2uint_float(pfc[1],pfc[0]);

    Blynk.virtualWrite(V33, pf_c); 
    delay(milis);
  }
  data32 = node1.readHoldingRegisters(3083, 2);
  if (data32 == node1.ku8MBSuccess) 
  {
    pft[0] = node1.getResponseBuffer(0x00);
    pft[1] = node1.getResponseBuffer(0x01);
    pf_tot = f_2uint_float(pft[1],pft[0]);

    Blynk.virtualWrite(V34, pf_tot); 
    delay(milis);
  }
//END OF PF===========================================================================================================================

//ENERGY===========================================================================================================================
  data33 = node1.readHoldingRegisters(3203, 4);
  if (data33 == node1.ku8MBSuccess) 
  {
    aedd[0] = node1.getResponseBuffer(0x00);
    aedd[1] = node1.getResponseBuffer(0x01);
    aedd[2] = node1.getResponseBuffer(0x02);
    aedd[3] = node1.getResponseBuffer(0x03);
    aed = l_4uint_int64(aedd[3],aedd[2],aedd[1],aedd[0]);

    Blynk.virtualWrite(V35, aed); 
    delay(milis);
  }
  data34 = node1.readHoldingRegisters(3207, 4);
  if (data34 == node1.ku8MBSuccess) 
  {
    aerr[0] = node1.getResponseBuffer(0x00);
    aerr[1] = node1.getResponseBuffer(0x01);
    aerr[2] = node1.getResponseBuffer(0x02);
    aerr[3] = node1.getResponseBuffer(0x03);
    aer = l_4uint_int64(aerr[3],aerr[2],aerr[1],aerr[0]);

    Blynk.virtualWrite(V36, aer); 
    delay(milis);
  }
  data35 = node1.readHoldingRegisters(3219, 4);
  if (data35 == node1.ku8MBSuccess) 
  {
    redd[0] = node1.getResponseBuffer(0x00);
    redd[1] = node1.getResponseBuffer(0x01);
    redd[2] = node1.getResponseBuffer(0x02);
    redd[3] = node1.getResponseBuffer(0x03);
    red = l_4uint_int64(redd[3],redd[2],redd[1],redd[0]);

    Blynk.virtualWrite(V37, red); 
    delay(milis);
  }
  data36 = node1.readHoldingRegisters(3223, 4);
  if (data36 == node1.ku8MBSuccess) 
  {
    rerr[0] = node1.getResponseBuffer(0x00);
    rerr[1] = node1.getResponseBuffer(0x01);
    rerr[2] = node1.getResponseBuffer(0x02);
    rerr[3] = node1.getResponseBuffer(0x03);
    rer = l_4uint_int64(rerr[3],rerr[2],rerr[1],rerr[0]);

    Blynk.virtualWrite(V38, rer); 
    delay(milis);
  }
  data37 = node1.readHoldingRegisters(3235, 4);
  if (data37 == node1.ku8MBSuccess) 
  {
    apedd[0] = node1.getResponseBuffer(0x00);
    apedd[1] = node1.getResponseBuffer(0x01);
    apedd[2] = node1.getResponseBuffer(0x02);
    apedd[3] = node1.getResponseBuffer(0x03);
    aped = l_4uint_int64(apedd[3],apedd[2],apedd[1],apedd[0]);

    Blynk.virtualWrite(V39, aped); 
    delay(milis);
  }
  data38 = node1.readHoldingRegisters(3239, 4);
  if (data38 == node1.ku8MBSuccess) 
  {
    aperr[0] = node1.getResponseBuffer(0x00);
    aperr[1] = node1.getResponseBuffer(0x01);
    aperr[2] = node1.getResponseBuffer(0x02);
    aperr[3] = node1.getResponseBuffer(0x03);
    aper = l_4uint_int64(aperr[3],aperr[2],aperr[1],aperr[0]);

    Blynk.virtualWrite(V40, aper); 
    delay(milis);
  }
//END OF ENERGY===========================================================================================================================

//CURRENT THD===========================================================================================================================
  data39 = node1.readHoldingRegisters(21327, 2);
  if (data39 == node1.ku8MBSuccess) 
  {
    cthdll[0] = node1.getResponseBuffer(0x00);
    cthdll[1] = node1.getResponseBuffer(0x01);
    cthd_ll = f_2uint_float(cthdll[1],cthdll[0]);

    Blynk.virtualWrite(V41, cthd_ll); 
    delay(milis);
  }
  data40 = node1.readHoldingRegisters(21337, 2);
  if (data40 == node1.ku8MBSuccess) 
  {
    cthdln[0] = node1.getResponseBuffer(0x00);
    cthdln[1] = node1.getResponseBuffer(0x01);
    cthd_ln = f_2uint_float(cthdln[1],cthdln[0]);

    Blynk.virtualWrite(V42, cthd_ln); 
    delay(milis);
  }
  data41 = node1.readHoldingRegisters(21299, 2);
  if (data41 == node1.ku8MBSuccess) 
  {
    cthda[0] = node1.getResponseBuffer(0x00);
    cthda[1] = node1.getResponseBuffer(0x01);
    cthd_a = f_2uint_float(cthda[1],cthda[0]);

    Blynk.virtualWrite(V43, cthd_a); 
    delay(milis);
  }
  data42 = node1.readHoldingRegisters(21301, 2);
  if (data42 == node1.ku8MBSuccess) 
  {
    cthdb[0] = node1.getResponseBuffer(0x00);
    cthdb[1] = node1.getResponseBuffer(0x01);
    cthd_b = f_2uint_float(cthdb[1],cthdb[0]);

    Blynk.virtualWrite(V44, cthd_b); 
    delay(milis);
  }
  data43 = node1.readHoldingRegisters(21303, 2);
  if (data43 == node1.ku8MBSuccess) 
  {
    cthdc[0] = node1.getResponseBuffer(0x00);
    cthdc[1] = node1.getResponseBuffer(0x01);
    cthd_c = f_2uint_float(cthdc[1],cthdc[0]);

    Blynk.virtualWrite(V45, cthd_c); 
    delay(milis);
  }
  data44 = node1.readHoldingRegisters(21305, 2);
  if (data44 == node1.ku8MBSuccess) 
  {
    cthdn[0] = node1.getResponseBuffer(0x00);
    cthdn[1] = node1.getResponseBuffer(0x01);
    cthd_n = f_2uint_float(cthdn[1],cthdn[0]);

    Blynk.virtualWrite(V46, cthd_n); 
    delay(milis);
  }
  data45 = node1.readHoldingRegisters(21307, 2);
  if (data45 == node1.ku8MBSuccess) 
  {
    cthdg[0] = node1.getResponseBuffer(0x00);
    cthdg[1] = node1.getResponseBuffer(0x01);
    cthd_g = f_2uint_float(cthdg[1],cthdg[0]);

    Blynk.virtualWrite(V47, cthd_g); 
    delay(milis);
  }
//END OF CURRENT THD===========================================================================================================================

//VOLTAGE THD===========================================================================================================================
  data46 = node1.readHoldingRegisters(21329, 2);
  if (data46 == node1.ku8MBSuccess) 
  {
    vthdan[0] = node1.getResponseBuffer(0x00);
    vthdan[1] = node1.getResponseBuffer(0x01);
    vthd_an = f_2uint_float(vthdan[1],vthdan[0]);

    Blynk.virtualWrite(V48, vthd_an); 
    delay(milis);
  }
  data47 = node1.readHoldingRegisters(21331, 2);
  if (data47 == node1.ku8MBSuccess) 
  {
    vthdbn[0] = node1.getResponseBuffer(0x00);
    vthdbn[1] = node1.getResponseBuffer(0x01);
    vthd_bn = f_2uint_float(vthdbn[1],vthdbn[0]);

    Blynk.virtualWrite(V49, vthd_bn); 
    delay(milis);
  }
  data48 = node1.readHoldingRegisters(21333, 2);
  if (data48 == node1.ku8MBSuccess) 
  {
    vthdcn[0] = node1.getResponseBuffer(0x00);
    vthdcn[1] = node1.getResponseBuffer(0x01);
    vthd_cn = f_2uint_float(vthdcn[1],vthdcn[0]);

    Blynk.virtualWrite(V50, vthd_cn); 
    delay(milis);
  }
  data49 = node1.readHoldingRegisters(21337, 2);
  if (data49 == node1.ku8MBSuccess) 
  {
    vthdln[0] = node1.getResponseBuffer(0x00);
    vthdln[1] = node1.getResponseBuffer(0x01);
    vthd_ln = f_2uint_float(vthdln[1],vthdln[0]);

    Blynk.virtualWrite(V51, vthd_ln); 
    delay(milis);
  }
  data50 = node1.readHoldingRegisters(21321, 2);
  if (data50 == node1.ku8MBSuccess) 
  {
    vthdab[0] = node1.getResponseBuffer(0x00);
    vthdab[1] = node1.getResponseBuffer(0x01);
    vthd_ab = f_2uint_float(vthdab[1],vthdab[0]);

    Blynk.virtualWrite(V52, vthd_ab); 
    delay(milis);
  }
  data51 = node1.readHoldingRegisters(21323, 2);
  if (data51 == node1.ku8MBSuccess) 
  {
    vthdbc[0] = node1.getResponseBuffer(0x00);
    vthdbc[1] = node1.getResponseBuffer(0x01);
    vthd_bc = f_2uint_float(vthdbc[1],vthdbc[0]);

    Blynk.virtualWrite(V53, vthd_bc); 
    delay(milis);
  }
  data52 = node1.readHoldingRegisters(21325, 2);
  if (data52 == node1.ku8MBSuccess) 
  {
    vthdca[0] = node1.getResponseBuffer(0x00);
    vthdca[1] = node1.getResponseBuffer(0x01);
    vthd_ca = f_2uint_float(vthdca[1],vthdca[0]);

    Blynk.virtualWrite(V54, vthd_ca); 
    delay(milis);
  }
  data53 = node1.readHoldingRegisters(21327, 2);
  if (data53 == node1.ku8MBSuccess) 
  {
    vthdll[0] = node1.getResponseBuffer(0x00);
    vthdll[1] = node1.getResponseBuffer(0x01);
    vthd_ll = f_2uint_float(vthdll[1],vthdll[0]);

    Blynk.virtualWrite(V55, vthd_ll); 
    delay(milis);
  }
//END OF VOLTAGE THD===========================================================================================================================

//MINIMUM VOLTAGE VALUES===========================================================================================================================
  data54 = node1.readHoldingRegisters(27237, 2);
  if (data54 == node1.ku8MBSuccess) 
  {
    minvab[0] = node1.getResponseBuffer(0x00); 
    minvab[1] = node1.getResponseBuffer(0x01);
    minv_ab = f_2uint_float(minvab[1], minvab[0]);

    Blynk.virtualWrite(V56, minv_ab); 
    delay(milis);
  }
  data55 = node1.readHoldingRegisters(27239, 2);
  if (data55 == node1.ku8MBSuccess) 
  {
    minvbc[0] = node1.getResponseBuffer(0x00);
    minvbc[1] = node1.getResponseBuffer(0x01);
    minv_bc = f_2uint_float(minvbc[1], minvbc[0]);

    Blynk.virtualWrite(V57, minv_bc); 
    delay(milis);
  }
  data56 = node1.readHoldingRegisters(27241, 2);
  if (data56 == node1.ku8MBSuccess) 
  {
    minvca[0] = node1.getResponseBuffer(0x00);
    minvca[1] = node1.getResponseBuffer(0x01);
    minv_ca = f_2uint_float(minvca[1], minvca[0]);

    Blynk.virtualWrite(V58, minv_ca); 
    delay(milis);
  }
  data57 = node1.readHoldingRegisters(27243, 2);
  if (data57 == node1.ku8MBSuccess) 
  {
    minvll[0] = node1.getResponseBuffer(0x00);
    minvll[1] = node1.getResponseBuffer(0x01);
    minv_ll = f_2uint_float(minvll[1], minvll[0]);

    Blynk.virtualWrite(V59, minv_ll); 
    delay(milis);
  }
  data58 = node1.readHoldingRegisters(27245, 2);
  if (data58 == node1.ku8MBSuccess) 
  {
    minvan[0] = node1.getResponseBuffer(0x00);
    minvan[1] = node1.getResponseBuffer(0x01);
    minv_an = f_2uint_float(minvan[1], minvan[0]);

    Blynk.virtualWrite(V60, minv_an); 
    delay(milis);
  }
  data59 = node1.readHoldingRegisters(27247, 2);
  if (data59 == node1.ku8MBSuccess) 
  {
    minvbn[0] = node1.getResponseBuffer(0x00);
    minvbn[1] = node1.getResponseBuffer(0x01);
    minv_bn = f_2uint_float(minvbn[1], minvbn[0]);

    Blynk.virtualWrite(V61, minv_bn); 
    delay(milis);
  }
  data60 = node1.readHoldingRegisters(27249, 2);
  if (data60 == node1.ku8MBSuccess) 
  {
    minvcn[0] = node1.getResponseBuffer(0x00);
    minvcn[1] = node1.getResponseBuffer(0x01);
    minv_cn = f_2uint_float(minvcn[1], minvcn[0]);

    Blynk.virtualWrite(V62, minv_cn); 
    delay(milis);
  }
  data61 = node1.readHoldingRegisters(27253, 2); 
  if (data61 == node1.ku8MBSuccess) 
  {
    minvln[0] = node1.getResponseBuffer(0x00);
    minvln[1] = node1.getResponseBuffer(0x01);
    minv_ln = f_2uint_float(minvln[1], minvln[0]);

    Blynk.virtualWrite(V63, minv_ln); 
    delay(milis);
  }
//END OF MINIMUM VOLTAGE VALUES===========================================================================================================================

//MAXIMUM VOLTAGE VALUES===========================================================================================================================
  data62 = node1.readHoldingRegisters(27713, 2);
  if (data62 == node1.ku8MBSuccess) 
  {
    maxvab[0] = node1.getResponseBuffer(0x00);
    maxvab[1] = node1.getResponseBuffer(0x01);
    maxv_ab = f_2uint_float(maxvab[1], maxvab[0]);

    Blynk.virtualWrite(V64, maxv_ab); 
    delay(milis);
  }
  data63 = node1.readHoldingRegisters(27715, 2);
  if (data63 == node1.ku8MBSuccess) 
  {
    maxvbc[0] = node1.getResponseBuffer(0x00);
    maxvbc[1] = node1.getResponseBuffer(0x01);
    maxv_bc = f_2uint_float(maxvbc[1], maxvbc[0]);

    Blynk.virtualWrite(V65, maxv_bc); 
    delay(milis);
  }
  data64 = node1.readHoldingRegisters(27717, 2);
  if (data64 == node1.ku8MBSuccess) 
  {
    maxvca[0] = node1.getResponseBuffer(0x00);
    maxvca[1] = node1.getResponseBuffer(0x01);
    maxv_ca = f_2uint_float(maxvca[1], maxvca[0]);

    Blynk.virtualWrite(V66, maxv_ca); 
    delay(milis);
  }
  data65 = node1.readHoldingRegisters(27719, 2);
  if (data65 == node1.ku8MBSuccess) 
  {
    maxvll[0] = node1.getResponseBuffer(0x00);
    maxvll[1] = node1.getResponseBuffer(0x01);
    maxv_ll = f_2uint_float(maxvll[1], maxvll[0]);

    Blynk.virtualWrite(V67, maxv_ll); 
    delay(milis);
  }
  data66 = node1.readHoldingRegisters(27721, 2);
  if (data66 == node1.ku8MBSuccess) 
  {
    maxvan[0] = node1.getResponseBuffer(0x00);
    maxvan[1] = node1.getResponseBuffer(0x01);
    maxv_an = f_2uint_float(maxvan[1], maxvan[0]);

    Blynk.virtualWrite(V68, maxv_an); 
    delay(milis);
  }
  data67 = node1.readHoldingRegisters(27723, 2);
  if (data67 == node1.ku8MBSuccess) 
  {
    maxvbn[0] = node1.getResponseBuffer(0x00);
    maxvbn[1] = node1.getResponseBuffer(0x01);
    maxv_bn = f_2uint_float(maxvbn[1], maxvbn[0]);

    Blynk.virtualWrite(V69, maxv_bn); 
    delay(milis);
  }
  data68 = node1.readHoldingRegisters(27725, 2);
  if (data68 == node1.ku8MBSuccess) 
  {
    maxvcn[0] = node1.getResponseBuffer(0x00);
    maxvcn[1] = node1.getResponseBuffer(0x01);
    maxv_cn = f_2uint_float(maxvcn[1], maxvcn[0]);

    Blynk.virtualWrite(V70, maxv_cn); 
    delay(milis);
  }
  data69 = node1.readHoldingRegisters(27729, 2); // Changed address for example
  if (data69 == node1.ku8MBSuccess) 
  {
    maxvln[0] = node1.getResponseBuffer(0x00);
    maxvln[1] = node1.getResponseBuffer(0x01);
    maxv_ln = f_2uint_float(maxvln[1], maxvln[0]);

    Blynk.virtualWrite(V71, maxv_ln); 
    delay(milis);
  }
//END OF MAXIMUM VOLTAGE VALUES===========================================================================================================================

//CURRENT===========================================================================================================================
  data70 = node1.readHoldingRegisters(2999, 2);
  if (data70 == node1.ku8MBSuccess) 
  {
    aa[0] = node1.getResponseBuffer(0x00);
    aa[1] = node1.getResponseBuffer(0x01);
    amp_a = f_2uint_float(aa[1],aa[0]);

    Blynk.virtualWrite(V72, amp_a); 
    delay(milis);
  }
  data71 = node1.readHoldingRegisters(3001, 2);
  if (data71 == node1.ku8MBSuccess) 
  {
    ab[0] = node1.getResponseBuffer(0x00);
    ab[1] = node1.getResponseBuffer(0x01);
    amp_b = f_2uint_float(ab[1],ab[0]);

    Blynk.virtualWrite(V73, amp_b); 
    delay(milis);
  }
  data72 = node1.readHoldingRegisters(3003, 2);
  if (data72 == node1.ku8MBSuccess) 
  {
    ac[0] = node1.getResponseBuffer(0x00);
    ac[1] = node1.getResponseBuffer(0x01);
    amp_c = f_2uint_float(ac[1],ac[0]);

    Blynk.virtualWrite(V74, amp_c); 
    delay(milis);
  }
  data73 = node1.readHoldingRegisters(3005, 2);
  if (data73 == node1.ku8MBSuccess) 
  {
    an[0] = node1.getResponseBuffer(0x00);
    an[1] = node1.getResponseBuffer(0x01);
    amp_n = f_2uint_float(an[1],an[0]);

    Blynk.virtualWrite(V75, amp_n); 
    delay(milis);
  }
  data74 = node1.readHoldingRegisters(3007, 2);
  if (data74 == node1.ku8MBSuccess) 
  {
    ag[0] = node1.getResponseBuffer(0x00);
    ag[1] = node1.getResponseBuffer(0x01);
    amp_g = f_2uint_float(ag[1],ag[0]);

    Blynk.virtualWrite(V76, amp_g); 
    delay(milis);
  }
  data75 = node1.readHoldingRegisters(3009, 2);
  if (data75 == node1.ku8MBSuccess) 
  {
    aavg[0] = node1.getResponseBuffer(0x00);
    aavg[1] = node1.getResponseBuffer(0x01);
    amp_avg = f_2uint_float(aavg[1],aavg[0]);

    Blynk.virtualWrite(V77, amp_avg); 
    delay(milis);
  }
//END OF CURRENT===========================================================================================================================

//CURRENT UNBALANCE===========================================================================================================================
  data76 = node1.readHoldingRegisters(3011, 2);
  if (data76 == node1.ku8MBSuccess) 
  {
    uba[0] = node1.getResponseBuffer(0x00);
    uba[1] = node1.getResponseBuffer(0x01);
    cunbal_a = f_2uint_float(uba[1],uba[0]);

    Blynk.virtualWrite(V78, cunbal_a); 
    delay(milis);
  }
  data77 = node1.readHoldingRegisters(3013, 2);
  if (data77 == node1.ku8MBSuccess) 
  {
    ubb[0] = node1.getResponseBuffer(0x00);
    ubb[1] = node1.getResponseBuffer(0x01);
    cunbal_b = f_2uint_float(ubb[1],ubb[0]);

    Blynk.virtualWrite(V79, cunbal_b); 
    delay(milis);
  }
  data78 = node1.readHoldingRegisters(3015, 2);
  if (data78 == node1.ku8MBSuccess) 
  {
    ubc[0] = node1.getResponseBuffer(0x00);
    ubc[1] = node1.getResponseBuffer(0x01);
    cunbal_c = f_2uint_float(ubc[1],ubc[0]);

    Blynk.virtualWrite(V80, cunbal_c); 
    delay(milis);
  }
  data79 = node1.readHoldingRegisters(3017, 2);
  if (data79 == node1.ku8MBSuccess) 
  {
    ubw[0] = node1.getResponseBuffer(0x00);
    ubw[1] = node1.getResponseBuffer(0x01);
    cunbal_wrst = f_2uint_float(ubw[1],ubw[0]);

    Blynk.virtualWrite(V81, cunbal_wrst); 
    delay(milis);
  }
}
//END OF CURRENT UNBALANCE===========================================================================================================================
void PERHITUNGAN(){
  double klasifikasi, klasifikasi2;
  double a = 7.84809436e-01,  b= 2.23257526e+01,  c= 6.25949666e-03, d= -7.48525121e-02,e=4.46890595e+01, f=-3.02958687e+01, g=-1.42921461e+00, h= -752.640457501826;
  //perhitungan klasifikasi
  klasifikasi = a*volt_lnavg + b*amp_avg + c*aed+ d*actpo_tot + e*cthd_ln + f*vthd_ln + g*pf_tot +h;

 if (klasifikasi >= 0 && klasifikasi <=15){
    Blynk.virtualWrite(V82, "Beban Lain");
    }

    else if (klasifikasi >= 15 && klasifikasi <=35){
    Blynk.virtualWrite(V82, "Lampu");
    }

    else if (klasifikasi >= 35 && klasifikasi <= 50){
    Blynk.virtualWrite(V82, "AC");
    }
  Blynk.virtualWrite(V83, klasifikasi);
  delay(milis);

  double a2=1.03258485e+00, b2=-4.10079223e+00,  c2=1.04988345e-03,  d2=4.16700986e-03,e2=-2.02901344e+00, f2=1.45025815e+01, g2=-6.90011102e+00,  h2=3.11619823e-01, i2=-403.2845804331244;
  klasifikasi2 = a2*volt_lnavg + b2*amp_avg + c2*aed+ d2*actpo_tot + e2*cthd_ln + f2*vthd_ln + g2*pf_tot +h2*klasifikasi +i2;

  if (klasifikasi2 >= 0 && klasifikasi2 <=15){
    Blynk.virtualWrite(V84, "Beban Lain");
    }

    else if (klasifikasi2 >= 15 && klasifikasi2 <=35){
    Blynk.virtualWrite(V84, "Lampu");
    }

    else if (klasifikasi2 >= 35 && klasifikasi2 <= 50){
    Blynk.virtualWrite(V84, "AC");
    }
  Blynk.virtualWrite(V85, klasifikasi2);
  delay(milis);
}                                                     
unsigned long lastVSCHNEIDER = 0;
unsigned long lastPERHITUNGAN = 0;

void loop() {
  BlynkEdgent.run();

  unsigned long currentMillis = millis();

  if (currentMillis - lastVSCHNEIDER >= 200) {
    VSCHNEIDER();
    lastVSCHNEIDER = currentMillis;
  }
  if (currentMillis - lastPERHITUNGAN >= 200) {
    PERHITUNGAN();
    lastPERHITUNGAN = currentMillis;
  }
}

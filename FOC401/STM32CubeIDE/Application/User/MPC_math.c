#include "main.h"

const uint16_t sinTable[] = {0,9,18,27,36,45,54,62,71,80,89,98,106,115,124,133,141,150,158,167,175,183,192,200,208,216,224,232,240,248,256,264,271,279,286,294,301,308,315,322,329,336,343,349,356,362,368,374,380,386,392,398,403,409,414,419,424,429,434,439,443,448,452,456,460,464,468,471,475,478,481,484,487,490,492,495,497,499,501,503,504,506,507,508,509,510,511,511,512,512,512};
const uint16_t sinTableLowRes[] = {0,1,3,5,6,8,10,12,13,15,17,19,20,22,24,25,27,29,30,32,34,35,37,39,40,42,43,45,46,48,49,51,52,54,55,57,58,60,61,62,64,65,66,68,69,70,71,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,89,90,91,92,92,93,93,94,95,95,96,96,97,97,97,98,98,98,99,99,99,99,99,99,99,99,100};

short sin000,sin120,sin240,cos000,cos120,cos240;


/**
 * This function limits the range of theta between 0 and 360 degrees
 *
 * @param short theta
 * @return short angle between 0 to 360 degrees
 */
short limitTheta(short theta){
	if(theta < 0){
		return ((360+theta) - 360*(1+(theta/360)));
	} else {
		return (theta - 360*(theta/360));
	}
}

/**
 * This function computes sin(thetaElec) using LUT
 *
 * @param short thetaElec in degrees
 * @return short sin(x) range -512 to +512
 */
short sin2(short thetaElec){
  thetaElec = limitTheta(thetaElec);

  if(thetaElec <= 90){
    return sinTable[thetaElec];
  } else if(thetaElec > 90 && thetaElec <=180){
    return sinTable[180 - thetaElec];
  } else if(thetaElec > 180 && thetaElec <= 270){
    return -sinTable[thetaElec - 180];
  } else {
    return -sinTable[360 - thetaElec];
  }
}




/**
 * This function computes cos(theta) using LUT
 *
 * @param short theta in degrees
 * @return short cos(x) range -512 to +512
 */
short cos2(short theta){
  return sin2(theta+90);
}


/**
 * This function computes sine & cosine values for given thetaElec to be used in park transform
 *
 */
void computeSinCos(int16_t thetaElec){
	thetaElec += 90;
	sin000 = sin2(thetaElec);
	cos000 = cos2(thetaElec);
	sin120 = sin2(thetaElec+120);
	sin240 = sin2(thetaElec+240);
	cos120 = cos2(thetaElec+120);
	cos240 = cos2(thetaElec+240);
}


/**
 * This function computes Park transform
 *
 */
void parkTransform(short a, short b, short c, volatile qd_t *Xdq){
	Xdq->d = (sin000*a + sin240*b + sin120*c)*1000/768; // (2/3)*(1/512) = 1/768
	Xdq->q = (cos000*a + cos240*b + cos120*c)*1000/768;
}



/**
 * This function computes Clarke transform
 *
 */
void clarkeTransform(int16_t a, int16_t b, int16_t c, alphabeta_t *Xalbt){
	Xalbt->alpha = (a*2/3) - ((b+c)/3);
	Xalbt->beta = (b-c)*250/433;
}

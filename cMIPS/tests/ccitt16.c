
#ifdef cMIPS
  #include "cMIPS.h"
#else
  #include <stdio.h>
#endif

//#define NUM_ITEMS 1000
//#define NUM_ITEMS 100
//
#define NUM_ITEMS 50
//#define NUM_ITEMS 25
//#define NUM_ITEMS 10
//#define NUM_ITEMS 5

int ccitt16(int numbers[], unsigned int array_size);

void main() {

  int crc;

#ifdef cMIPS
  int *IO = (int *)x_IO_BASE_ADDR;
  int *DATA = (int *)x_DATA_BASE_ADDR;
#else
  int DATA[NUM_ITEMS];
#endif

  int *ptr = DATA;
  unsigned int i, m_w, m_z;

  // from wikipedia
  m_w = 17;    /* must not be zero, nor 0x464fffff */
  m_z = 31;    /* must not be zero, nor 0x9068ffff */

  for (i=0; i < NUM_ITEMS; i++, ptr++) {
    m_z = 36969 * (m_z & 65535) + (m_z >> 16);
    m_w = 18000 * (m_w & 65535) + (m_w >> 16);
    *ptr = (m_z << 16) + m_w;  /* 32-bit result */
  }

 
//w  int numbers[100] = {
//w 19570, 24222, 20446, 21065, 3087, 17014, 17830, 9159, 14257, 31179,
//w 2253, 30720, 18450, 8493, 32075, 9407, 32232, 19637, 13229, 32569,
//w 17414, 22005, 32399, 18901, 5937, 7328, 25562, 12989, 4035, 9722,
//w 26046, 12073, 26882, 18174, 24430, 16230, 8765, 17745, 29087, 26536,
//w 5548, 6889, 15038, 27029, 8453, 18556, 22260, 2701, 15326, 25259,
//w 
//w 25562, 29371, 6086, 17899, 1020, 28893, 9761, 7479, 7695, 11987,
//w 8135, 10104, 21490, 8937, 27228, 26543, 28007, 18916, 30573, 11240,
//w 12942, 30158, 31055, 5855, 29221, 15745, 2488, 6255, 12161, 22774,
//w 7706, 190, 13387, 30283, 26051, 6698, 508, 21398, 28086, 30194,
//w 24342, 10604, 9798, 22437, 31784, 12847, 9848, 27467, 27775, 23067,
//w 
//z 18710, 12884, 28332, 25616, 32659, 21316, 16570, 3869, 30370, 14364,
//z 31788, 2256, 26773, 10119, 13752, 14400, 14710, 17610, 23750, 11734,
//z 14567, 8808, 5799, 17376, 9657, 21400, 4502, 15316, 20303, 10344,
//z 2927, 25700, 28849, 173, 2097, 5189, 8559, 25010, 29749, 1471,
//z 30780, 15004, 11819, 24309, 15174, 926, 13530, 29426, 14934, 9186,
//z 29844, 1911, 11948, 18994, 28794, 11143, 15512, 9322, 17269, 27311,
//z 29400, 8606, 13551, 26901, 25525, 11019, 23893, 13383, 13276, 32702,
//z 5710, 21697, 15101, 7942, 12984, 27708, 4645, 28059, 14951, 27833,
//z 11228, 395, 9628, 13393, 20990, 16616, 21575, 6585, 9562, 16179,
//z 15612, 13719, 31500, 1415, 4023, 29832, 13210, 12677, 18512, 15854,
//z 26577, 5891, 27836, 15226, 1885, 20596, 29449, 3453, 17707, 27157,
//z 2840, 30702, 28031, 5085, 18455, 21562, 468, 1570, 26053, 21455,
//z 13943, 1291, 9573, 24423, 11746, 11961, 32383, 7956, 3545, 52,
//z 8026, 30480, 19014, 15639, 31033, 1346, 16853, 9723, 21592, 27536,
//z 2127, 10661, 10148, 29415, 31416, 31983, 12350, 21259, 31015, 17783,
//z 1198, 11040, 25192, 17161, 8167, 30452, 11603, 9165, 8191, 3346,
//z 15338, 28464, 13483, 29063, 17460, 13203, 31100, 2418, 27357, 25270,
//z 14299, 31565, 27530, 2275, 19653, 18430, 27390, 19162, 11847, 26781,
//z 20206, 22144, 19641, 32003, 14244, 12870, 19386, 22295, 30550, 5978,
//z 20207, 28376, 9027, 17040, 22138, 2858, 6803, 26639, 26428, 11104,
//z 7916, 13755, 31435, 32141, 23252, 4198, 29490, 28042, 29129, 30646,
//z 3691, 6343, 30242, 15572, 5533, 13011, 24740, 30681, 17430, 9897,
//z 26083, 26617, 8710, 192, 17759, 10861, 20225, 28113, 28250, 29730,
//z 30771, 23142, 5754, 14883, 30304, 32551, 7715, 29895, 20387, 26084,
//z 24210, 27034, 6370, 13153, 15802, 10252, 29460, 16869, 9183, 9436,
//z 8242, 22657, 12019, 8258, 27646, 2849, 16307, 17119, 25719, 3924,
//z 15489, 7525, 7703, 4571, 9728, 30470, 32722, 22771, 22897, 5790,
//z 21199, 16226, 7377, 29343, 4580, 21571, 20654, 29632, 13524, 11154,
//z 25549, 13925, 29512, 11127, 25725, 30413, 15246, 25171, 19385, 5117,
//z 22803, 12683, 24065, 21872, 1697, 22807, 15861, 24443, 26713, 12233,
//z 3254, 642, 10608, 5214, 13308, 20701, 14761, 25414, 13815, 10120,
//z 26262, 32051, 7911, 5516, 19598, 10851, 29818, 20017, 23405, 26286,
//z 16105, 4576, 27407, 25167, 2001, 14810, 7050, 17266, 27346, 21147,
//z 32045, 29825, 5097, 28940, 15420, 2863, 18318, 11883, 19561, 5185,
//z 20621, 30459, 20499, 20511, 28580, 5995, 14142, 11294, 10465, 13575,
//z 21077, 1162, 2746, 7328, 21665, 31607, 19177, 5180, 648, 28741,
//z 21774, 7038, 1926, 25588, 9986, 18132, 12269, 22061, 5884, 8546,
//z 604, 26379, 24422, 16818, 29953, 18862, 3197, 32024, 14990, 21375,
//z 10155, 26086, 25340, 13376, 15524, 85, 19458, 8922, 5186, 23086,
//z 7508, 2790, 16673, 28987, 19612, 1785, 23034, 5233, 25913, 13232,
//z 32192, 11226, 4026, 10111, 32125, 4932, 5401, 6968, 18409, 29612,
//x 22967, 3933, 29391, 18280, 3099, 31271, 32288, 29974, 875, 19027,
//x 11476, 11591, 2470, 19119, 3972, 30443, 3495, 12904, 7369, 5756,
//x 30938, 28176, 29522, 15339, 9403, 28018, 8647, 9035, 11283, 534,
//x 4193, 5182, 5106, 6802, 26694, 27682, 2155, 26263, 5285, 7932,
//x 10514, 12215, 14742, 23329, 8502, 29431, 15535, 15270, 2015, 1817,
//x 15205, 13709, 17286, 23462, 2857, 11180, 28055, 7357, 7998, 21852,
//x 29095, 18334, 28764, 25575, 25717, 31734, 16903, 4783, 20130, 5608,
//x 25004, 3614, 26416, 23775, 15232, 23889, 31164, 21270, 24857, 14980,
//x 29819, 29156, 24009, 1596, 23030, 24265, 29885, 16694, 4741, 25913,
//x 9434, 14610, 22925, 16682, 3667, 21439, 17017, 1631, 5642, 9637,
//x 4473, 12402, 25260, 16022, 25597, 10148, 3979, 22898, 25840, 18021,
//x 12602, 23196, 2229, 11170, 2026, 18720, 3298, 26065, 31623, 11936,
//x 14271, 17085, 15762, 2563, 4989, 20497, 11453, 16605, 8502, 22695,
//x 22088, 15261, 16296, 27278, 12574, 6265, 16817, 12765, 28334, 8474,
//x 28810, 13370, 32138, 28423, 30147, 14633, 6594, 27346, 26882, 27414,
//x 1791, 4612, 32123, 32033, 20574, 12233, 2863, 25003, 10770, 29787,
//x 28644, 23972, 20664, 5935, 999, 1171, 633, 28642, 7812, 26978,
//x 28130, 12770, 26178, 4079, 12931, 14044, 26847, 27475, 20297, 1001,
//x 14094, 29936, 6811, 30072, 32755, 10301, 8247, 15865, 26714, 28497,
//x 203, 9039, 6587, 19504, 12115, 9971, 17646, 19161, 10293, 1255,
//x 5294, 2059, 32286, 13665, 19407, 22747, 22682, 23860, 2879, 24311,
//x 16502, 22782, 31678, 5559, 21974, 20244, 22365, 9193, 19477, 29742,
//x 3768, 32345, 18271, 28369, 1523, 10018, 30736, 1408, 9589, 5560,
//x 4043, 929, 14951, 5041, 27738, 305, 25323, 6742, 5193, 31015,
//x 541, 31981, 24897, 23209, 30666, 27588, 29733, 242, 6130, 7166,
//x 20206, 15870, 7680, 8221, 12359, 25594, 6084, 15699, 3304, 31086,
//x 1076, 18824, 17075, 1064, 8679, 24106, 24975, 5864, 10573, 27577,
//x 9736, 3533, 28335, 31246, 419, 9602, 10218, 8558, 19714, 22824,
//x 20971, 2130, 32700, 13058, 29161, 29172, 2554, 31714, 11877, 10649,
//x 11218, 30039, 31715, 22177, 26211, 2707, 22885, 1360, 25269, 9246,
//x 22700, 28762, 27214, 2621, 28206, 22320, 19859, 544, 17622, 27917,
//x 21477, 4558, 4478, 11455, 4471, 5841, 8749, 7478, 16113, 7073,
//x 9488, 21198, 28358, 6097, 25402, 14684, 15446, 1787, 27419, 29434,
//x 20675, 621, 26306, 22419, 22931, 18832, 6610, 24877, 19722, 25884,
//x 17222, 4345, 6213, 20734, 24292, 6428, 4886, 22788, 642, 22331,
//x 21923, 5198, 21244, 27593, 19389, 11125, 19696, 31248, 8705, 21880,
//x 23872, 23808, 26801, 14187, 1349, 23739, 2128, 4830, 1747, 12508,
//x 25599, 28430, 21489, 23435, 17818, 10446, 15957, 10562, 28562, 5427,
//x 8866, 24924, 28808, 8962, 2912, 29538, 5294, 9946, 1983, 11541,
//x 4318, 15906, 12811, 17723, 17272, 29872, 8686, 18218, 9980, 6462,
//x 18025, 11029, 21099, 27789, 25489, 10010, 9053, 5974, 12297, 1287,
//x 28130, 4854, 20035, 2288, 19993, 28885, 7542, 13425, 30610, 11418,
//x 7774, 29379, 7781, 29968, 26550, 31105, 22031, 6102, 26230, 27192,
//x 527, 31689, 26060, 177, 13381, 22066, 6876, 23037, 5381, 1712,
//x 12344, 8261, 12542, 17892, 3358, 10062, 18638, 4830, 21620, 17871,
//x 32353, 2890, 22404, 30579, 31757, 6746, 30839, 5036, 29038, 10867,
//x 14483, 9534, 515, 17815, 28212, 11910, 389, 20629, 24204, 26254,
//x 6667, 19698, 14106, 15059, 14303, 19862, 8939, 8656, 22301, 2614,
//x 30612, 2642, 3857, 15749, 13802, 29563, 30542, 14874, 23176, 31123,
//w 
//w };

  int *numbers = DATA;

  //compute packet's CRC
  crc = ccitt16(numbers, NUM_ITEMS);

#ifdef cMIPS
  *IO = crc;             // 
#else
  printf("%08x\n",crc);  // 100: 05ed006a 100.525ns
#endif                   //  50: c711210b  58.025ns
                         //  25: 1ec3d3e2  36.775ns
                         //  10: 87c90a60  24.025ns
}                        //   5: 008ec173  19.775ns

int ccitt16 (int buffer[], unsigned int bufsize) {
  int crc = 0xFFFF;
  int P0;

  while (bufsize--) {
    crc  = (crc >> 8) | (crc << 8);
    crc ^= *buffer++;
    crc ^= (crc & 0xff) >> 4;
    crc ^= crc << 12;
    crc ^= (crc & 0xff) << 5;
  }
  return(crc);

}

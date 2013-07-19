#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "stream-acq.h"

void itoa(int num, unsigned int max_count, char str[]); 
 

char strn[6];
char strs[6];

int main(s32 argc, s8* argv[])
{

	int n=(atoi(argv[2])*atoi(argv[4]));

	int r = n/(atoi(argv[6])+1);


	char p1[121] = "./video_decode_io2_dm3730.x470MV -c h264dec -n ";
	char p2[121] = "./video_decode_io2_dm3730.x470MV -c h264dec -n ";

	if(argc != 7) {
		printf("Incorrect Number of Arguments\n");
		exit(-1);
	}

	s32 status;

	status = system("rm -f /home/root/camera1/*");
	status = system("rm -f /home/root/camera2/*");
	
	itoa(n, 4, strn);
	itoa(r, 4, strs);


	status = cam1("192.168.1.128", atoi(argv[4]));
	status = cam2("192.168.1.160", atoi(argv[4]));
	

	strcat(p1, strn);
	strcat(p1, " -s ");
	strcat(p1, strs);
	strcat(p1, " -i /home/root/camera1/cam1.264 -o /home/root/camera1/cam1.yuv");



	status = system(p1);

	status = system("./image_encode_io1_dm3730.x470MV -c jpegenc -e encode -i /home/root/camera1/cam1.yuv -o /home/root/camera1/cam1.jpeg -r 1280x720 --iColorSpace 3 --oColorSpace 1");

	strcat(p2, strn);
	strcat(p2, " -s ");
	strcat(p2, strs);
	strcat(p2, " -i /home/root/camera2/cam2.264 -o /home/root/camera2/cam2.yuv");



	status = system(p2);

	status = system("./image_encode_io1_dm3730.x470MV -c jpegenc -e encode -i /home/root/camera2/cam2.yuv -o /home/root/camera2/cam2.jpeg -r 1280x720 --iColorSpace 3 --oColorSpace 1");
	

	return 0;
}

void itoa(int num, unsigned int max_count, char str[])
{
        int i;
        static unsigned int count;
        static unsigned int compare = 1;
        if(count == 0){
                for(i = 0; i < (max_count - 1); i++)
                        compare *= 10;
        }

        if(count != max_count){
                if(num >= compare)
                        str[count] = num/compare + 0x30;
                else
                        str[count] = '0';
                count++;
                itoa((num % compare) * 10, max_count, str);
        }
        count = 0;
        compare = 1;
}


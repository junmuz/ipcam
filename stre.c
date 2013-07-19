#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "stream-acq.h"

s32 RecvStreamInit2(s32* sfd_rtp, s32* sfd_rtcp, s8* argv, s16* port_rtp, s16* port_rtcp)
{
	struct sockaddr_in cliaddr_rtp, servaddr_rtcp;

        if( 0 > (*sfd_rtp = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))) {
                #ifdef DEBUG
                perror("Socket Initialization Failed");
                #endif
                return FAIL;
        }
	
        bzero(&cliaddr_rtp, sizeof(struct sockaddr_in));
        cliaddr_rtp.sin_family = AF_INET;
        cliaddr_rtp.sin_port = htons(37426);
	cliaddr_rtp.sin_addr.s_addr = INADDR_ANY;
	*port_rtp = cliaddr_rtp.sin_port;

	if(0 > (bind(*sfd_rtp, (struct sockaddr *) &cliaddr_rtp, sizeof(struct sockaddr_in)))) {
		#ifdef DEBUG
		perror("Unable to bind");
		#endif
		return FAIL;
	}

	return SUCCESS;
}

s32 StreamAcqInit2(s32* sfd, s8* argv)
{

	struct sockaddr_in servaddr;

        if( 0 > (*sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
		#ifdef DEBUG
                perror("Socket Initialization Failed");
		#endif
		return FAIL;
	}
	
        bzero(&servaddr, sizeof(struct sockaddr_in));
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(PORT_NUM);
        if(0 >= (inet_pton(AF_INET, argv, &servaddr.sin_addr))) {
		#ifdef DEBUG
                perror("Invalid IP address");
		#endif
		return FAIL;
	}

        if(0 > (connect(*sfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr_in)))) {
		#ifdef DEBUG
                perror("Socket Connection Failed");
		#endif
		return FAIL;
	}

	return SUCCESS;
}


s32 SetRTSPOpts2(rtspOpts* ropts){
	ropts->setupOpt = 1;
	ropts->playOpt = 1;
	ropts->pauseOpt = 1;
	ropts->teardownOpt = 1;
	ropts->optionsOpt = 1;
	ropts->announceOpt = 0;
	ropts->describeOpt = 1;
	ropts->recordOpt = 0;
	ropts->redirectOpt = 0;
	ropts->set_parameterOpt = 0;

	return SUCCESS;
}

s32 ReqRTSPOpts2(s32* sfd, s8* argv, s8* param){
	const s8 cmd1[] = "OPTIONS rtsp://";
	const s8 cmd2[] = ":554/av0_0 RTSP/1.0\r\nCSeq: 0\r\n\r\n";
	s8 tl[400];
	s8 rl[1500];
	s32 n;

	write(*sfd, "\r\n", 2);
	bzero(tl, 400);
	strcat(tl, cmd1);
	strcat(tl, argv);
	strcat(tl, cmd2);
        if(write(*sfd,tl, strlen(tl)) <= 0) {
		#ifdef DEBUG
                perror("Message Sending Failed");
		#endif
		return FAIL;
	}
        n = read(*sfd, rl, 1500);
                rl[n] = 0;
        if(n <= 0){
		#ifdef DEBUG
		perror("Message Reception Failed");
		#endif
		return FAIL;
	}
	return SUCCESS;  
}


s32 ReqRTSPDscrb2(s32* sfd, s8* argv, s8* param)
{
	const s8 cmd1[] = "DESCRIBE rtsp://";
	const s8 cmd2[] = ":554/av0_0 RTSP/1.0\r\nCSeq: 1\r\nAccept: application/sdp\r\n\r\n";
	s8 tl[400];
	s8 rl[1500];
	s32 n;

	bzero(tl, 400);
	strcat(tl, cmd1);
	strcat(tl, argv);
	strcat(tl, cmd2);

        if(write(*sfd,tl, strlen(tl)) <= 0) {
                #ifdef DEBUG
                perror("Message Sending Failed");
                #endif
                return FAIL;
        }
        n = read(*sfd, rl, 1500);
                rl[n] = 0;
        if(n <= 0){
                #ifdef DEBUG
                perror("Message Reception Failed");
                #endif
                return FAIL;
        }

	return SUCCESS;
}

s32 ReqRTSPSetup2(s32* sfd, s8* argv, s8* param, s16 port_rtp, s16 port_rtcp)
{
	const s8 cmd1[] = "SETUP rtsp://";
        const s8 cmd2[] = ":554/av0_0/trackID=1 RTSP/1.0\r\nCSeq: 2\r\nTransport: RTP/AVP;unicast;client_port=37426-37427\r\n\r\n";

       s8 tl[400], temp[10];
        s8 rl[1500];
        s32 n = 0;

        bzero(tl, 400);
        strcat(tl, cmd1);
        strcat(tl, argv);
        strcat(tl, cmd2);
        if(write(*sfd,tl, strlen(tl)) <= 0) {
                #ifdef DEBUG
                perror("Message Sending Failed");
                #endif
                return FAIL;
        }
        n = read(*sfd, rl, 1500);
                rl[n] = 0;
        if(n <= 0){
                #ifdef DEBUG
                perror("Message Reception Failed");
                #endif
                return FAIL;
        }

	{
		s32 i = 0;
		
		while(i < n) {
			if(0 == (strncmp(&rl[i], "Session: ", 9))) {
				*(param + 0) = rl[i + 9];
				*(param + 1) = rl[i + 10];
				*(param + 2) = rl[i + 11];
				*(param + 3) = rl[i + 12];
				*(param + 4) = rl[i + 13];
				*(param + 5) = rl[i + 14];
				*(param + 6) = rl[i + 15];
				*(param + 7) = rl[i + 16];	
				*(param + 8) = '\0';
				break;
			}

			else
				i++;
		}
	}
        return SUCCESS;
}

s32 ReqRTSPPlay2(s32* sfd, s8* argv, s8* param)
{
      	const s8 cmd1[] = "PLAY rtsp://";
        const s8 cmd2[] = ":554/av0_0 RTSP/1.0\r\nCSeq: 3\r\nSession: ";
	const s8 cmd3[] = "\r\nRange: npt=0.000-\r\n\r\n";

        s8 tl[400];
        s8 rl[1500];
        s32 n;

        bzero(tl, 400);
        strcat(tl, cmd1);
        strcat(tl, argv);
        strcat(tl, cmd2);
	strcat(tl, param);
	strcat(tl, cmd3);

        if(write(*sfd,tl, strlen(tl)) <= 0) {
                #ifdef DEBUG
                perror("Message Sending Failed");
                #endif
                return FAIL;
        }
        n = read(*sfd, rl, 1500);
                rl[n] = 0;
        if(n <= 0){
                #ifdef DEBUG
                perror("Message Reception Failed");
                #endif
                return FAIL;
        }

        return SUCCESS;
}

s32 ReqRTSPTeardown2(s32* sfd, s8* argv, s8* param)
{
	const s8 cmd1[] = "TEARDOWN rtsp://";
	const s8 cmd2[] = ":554/av0_0 RTSP/1.0\r\nCSeq:4\r\nSession: ";
	const s8 cmd3[] = "\r\n\r\n";

        s8 tl[400];
        s8 rl[1500];
        s32 n;

        bzero(tl, 400);
        strcat(tl, cmd1);
        strcat(tl, argv);
        strcat(tl, cmd2);
        strcat(tl, param);
        strcat(tl, cmd3);

        if(write(*sfd,tl, strlen(tl)) <= 0) {
                #ifdef DEBUG
                perror("Message Sending Failed");
                #endif
                return FAIL;
        }
        n = read(*sfd, rl, 1500);
                rl[n] = 0;
        if(n <= 0){
                #ifdef DEBUG
                perror("Message Reception Failed");
                #endif
                return FAIL;
        }
	
	return SUCCESS;
}


s32 ReceiveVideo()
{


	return SUCCESS;
}

s32 StreamAcqClose2(s32* sfd)
{
	close(*sfd);

	return SUCCESS;
}

u32 GetStream2(s32* sfd, FILE* fptr)
{
	const s8 cc[] = {0x00, 0x00, 0x00, 0x01};
	s8 rl[1500];
	s32 n;
	s32 marker = 0;
	u32 rettime = 0;
	fwrite(cc, 1, 4, fptr);
	while(marker == 0) {
	bzero(rl, 1500);
	n = read(*sfd, rl, 1500);


	rettime = (((u32) rl[4]) << 24) + (((u32) rl[5]) << 16) + (((u32) rl[6]) << 8) + (((u32) rl[7]));

	switch(rl[12]) {
		case 0x67:
		case 0x68:
		case 0x06:
			fwrite(&rl[12], 1, n - 12, fptr);
			fwrite(&cc, 1, 4, fptr);
			break;

		default:
			{
				s8 byte[2];
				bzero(byte, 2);
				if((rl[12] & 0x1F) == 0x1C) {
					if((rl[13] & 0x80)) {
						byte[0] = (rl[12] & 0xE0) | (rl[13] & 0x1F);
                                                fwrite(byte, 1, 1, fptr);
                                                fwrite(&rl[14],1, n-14, fptr);
  					}
				

					else {

                                                fwrite(&rl[14],1, n-14, fptr);

					}
				}
			}
			break;

	}

	if(rl[1] & 0x80) {
		marker = 1;
	}
	}

	return rettime;
}


s32 GetTimedStream2(s32* sfd, FILE* fptr, u32 time)
{
	u32 rettime = 0;
	u32 inittime;
	s32 status;
	inittime = GetStream2(sfd, fptr);
	while(rettime < (inittime + (time * 90000))) {
		rettime = GetStream2(sfd, fptr);
	}
	return SUCCESS;
}


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "stream-acq.h"

int cam1(s8* ipaddr, u32 time)
{
	s32 sfd, sfd_rtp, sfd_rtcp;
	s32 status;
	s16 port_rtp, port_rtcp;
	FILE* fptr;
	rtspOpts ropts;

	fptr = fopen("/home/root/camera1/cam1.264","w");

	if(NULL == fptr) {
		perror("Error opening File");
		exit(-1);
	}	

	// Init Socet
	// Init File open
	// IP address settings
	status = StreamAcqInit1(&sfd, ipaddr);
	status = RecvStreamInit1(&sfd_rtp, &sfd_rtcp, ipaddr, &port_rtp, &port_rtcp);

	if(SUCCESS != status) {
		perror("Unable to initialize stream Acquisition Module");
		exit(-1);
	}

	status = SetRTSPOpts1(&ropts);

        if(SUCCESS != status) {
                perror("Unable set RTSP Options");
                exit(-1);
        }


	if(ropts.optionsOpt) {
		status = ReqRTSPOpts1(&sfd, ipaddr, NULL);
	        if(SUCCESS != status) {
                	perror("Unable to send OPTIONS Req");
                	exit(-1);
        	}
	}

	if(ropts.describeOpt) {
		status = ReqRTSPDscrb1(&sfd, ipaddr, NULL);
                if(SUCCESS != status) {
                        perror("Unable to send DESCRIBE Req");
                        exit(-1);
                }
        }

	{

		char sess_num[9] = "01234567";
        	if(ropts.setupOpt) {
                	status = ReqRTSPSetup1(&sfd, ipaddr, sess_num, port_rtp, port_rtcp);
                	if(SUCCESS != status) {
                        	perror("Unable to send SETUP Req");
                        	exit(-1);
                	}
        	}

        	if(ropts.playOpt) {
                	status = ReqRTSPPlay1(&sfd, ipaddr, sess_num);
                	if(SUCCESS != status) {
                        	perror("Unable to send PLAY Req");
                        	exit(-1);
                	}
        	}

	


	//Get Options that are available
//	status = InitFile(fptr);

		printf("**Camera 1 Configured**\n");
	
		{
			s32 ind;
//			for(ind = 0; ind < 50; ind++) 
//				status = GetStream1(&sfd_rtp, fptr);
				status = GetTimedStream1(&sfd_rtp, fptr, time);
		}
	

		if(ropts.teardownOpt) {
			status = ReqRTSPTeardown1(&sfd, ipaddr, sess_num);
			if(SUCCESS != status) {
				perror("Unable to send TEARDOWN Req");
			}
		}
	}
	fclose(fptr);
	close(sfd);

	return 0;
}

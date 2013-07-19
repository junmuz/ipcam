
#define	DEBUG		1

#define	FAIL		0
#define SUCCESS		1

#define PORT_NUM	554

typedef	unsigned char	u8;
typedef	unsigned short	u16;
typedef unsigned int	u32;

typedef char		s8;
typedef short		s16;
typedef	int		s32;

typedef struct rtsp_options{
	s32 setupOpt:1;
	s32 playOpt:1;
	s32 pauseOpt:1;
	s32 teardownOpt:1;
	s32 optionsOpt:1;
	s32 announceOpt:1;
	s32 describeOpt:1;
	s32 recordOpt:1;
	s32 redirectOpt:1;
	s32 set_parameterOpt:1;
} rtspOpts;	

s32 ReceiveVideo();

s32 StreamAcqInit2(s32* sfd,  s8* argv);
s32 RecvStreamInit2(s32* sfd_rtp, s32* sfd_rtcp, s8* argv, s16* port_rtp, s16* port_rtcp);
s32 SetRTSPOpts2(rtspOpts* ropts);
s32 ReqRTSPOpts2(s32* sfd, s8* argv, s8* param);
s32 ReqRTSPDscrb2(s32* sfd, s8* argv, s8* param);
s32 ReqRTSPSetup2(s32* sfd, s8* argv, s8* param, s16 port_rtp, s16 port_rtcp);
s32 ReqRTSPPlay2(s32* sfd, s8* argv, s8* param);
s32 ReqRTSPTeardown2(s32* sfd, s8* argv, s8* param);
s32 StreamAcqClose2(s32* sfd);
u32 GetStream2(s32* sfd, FILE* fptr);
s32 GetTimedStream2(s32* sfd, FILE* fptr, u32 time);

s32 cam2(s8* ipaddr, u32 time);

s32 ReceiveVideo();

s32 StreamAcqInit1(s32* sfd,  s8* argv);
s32 RecvStreamInit1(s32* sfd_rtp, s32* sfd_rtcp, s8* argv, s16* port_rtp, s16* port_rtcp);
s32 SetRTSPOpts1(rtspOpts* ropts);
s32 ReqRTSPOpts1(s32* sfd, s8* argv, s8* param);
s32 ReqRTSPDscrb1(s32* sfd, s8* argv, s8* param);
s32 ReqRTSPSetup1(s32* sfd, s8* argv, s8* param, s16 port_rtp, s16 port_rtcp);
s32 ReqRTSPPlay1(s32* sfd, s8* argv, s8* param);
s32 ReqRTSPTeardown1(s32* sfd, s8* argv, s8* param);
s32 StreamAcqClose1(s32* sfd);
u32 GetStream1(s32* sfd, FILE* fptr);
s32 GetTimedStream1(s32* sfd, FILE* fptr, u32 time);

s32 cam1(s8* ipaddr, u32 time);


/*************************************HEAD FILES******************************/
#include"pid.h"

/******************************************************************************
Name��PID_Init 
Function:	
		  	initialize the PID value
Parameters��
		   	[in]	-	pid:pid structure
						Kp:P
						Ki:I
						Kd:D
Returns��
			void 
Description:
			null
******************************************************************************/
void PID_Init(pid_t pid, float Kp, float Ki, float Kd)
{
	pid->target = 0;
	pid->integral = 0;
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
}

/******************************************************************************
Name��PID_Cal_Ang 
Function:	
		  	calculate the angle PID result value
Parameters��
		   	[in]	-	*p:the pointer point to the angle PID structure
						current:current stage(angle)
						differential:the angle which is calculated every time
Returns��
			[out]	-	int32_t:the PID value output 
Description:
			null
******************************************************************************/
int32_t PID_Cal_Ang(pid_s *p, float current, float differential, int target)
{
	float offset;
	
	p->target = target;
	
	offset = p->target - current;
	p->integral += offset;

	return (int32_t)(p->Kp*offset + p->Ki*p->integral - p->Kd*differential);
}

/******************************************************************************
Name��PID_Cal_Speed 
Function:	
		  	calculate the speed PID result value
Parameters��
		   	[in]	-	*p:the pointer point to the speed PID structure
						current:current stage(speed)
Returns��
			[out]	-	int32_t:the PID value output 
Description:
			null
******************************************************************************/
int32_t spd_length = 0;
float spd_v = 0;	 //the speed after filter
int32_t last_target = 0;
int32_t PID_Cal_Speed(pid_s *p, int32_t current,int32_t target)
{
	int32_t temp;
	current = current + 0;//target speed is 0
	temp = (int32_t)(current/2);
	spd_v = spd_v * 4 / 5 + (float)(current) / 5;

	if(target == 0){
		if(last_target == 0)
			spd_length += (int32_t)temp;
		else
			spd_length = 0;
	} 
	if(target == 1){//forward
		spd_length += 1;
	}
	if(target == -1){//back
		spd_length -= 1;
	}

	if(spd_length > 1000)//1000
		spd_length = 1000;
	if(spd_length < -1000)
		spd_length = -1000;

	last_target = target;
//	printf("target:%d\r\n",target);
//	printf("spd=%.1lf\r\n",spd_v);
//	printf("l=%d\n",spd_length);
//	printf("tem=%.1lf\r\n",temp);	
	return (int32_t)(p->Kp * spd_v + p->Ki * (float)spd_length + p->Kd * temp);
}

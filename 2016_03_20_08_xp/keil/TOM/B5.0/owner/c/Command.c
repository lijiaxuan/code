#include "all.h"

double pos_error;
double ang_error;
double pwm_duty_1=0;

uint8_t SEND_CONVER_FLAG=0;

void R_CAMER_OFFSET_FUNC(void)//����λ��ƫ����Ϣ mm ��ƫΪ��
{
	pos_error =	MISO_CAMER_OFFSET.f64_data;
}

void R_CAMER_ANGLE_FUNC(void)//���սǶ�ƫ����Ϣ	 �� ��ƫΪ��
{
	ang_error =	MISO_CAMER_ANGLE.f64_data;
}

void W_PWM_FUNC(void)
{
// 	PWM1_SetDuty(MOSI_PWM.u16_data[0]);
// 	PWM2_SetDuty(MOSI_PWM.u16_data[1]);

    pwm_duty_1=MOSI_PWM.u16_data[0];
    pwm_duty_1=MOSI_PWM.u16_data[1];
    
    MISO_CMD=8;
    //Can_Database[HASH_TABLE[G_M_CMD_ID]].Data_ptr=0;
    Write_Database(G_M_CMD_ID);
}

//�ȿ�GPS������ô�� У�����ڳ�������ʱ��ʹ�õķ�ʽ
//��PID ��·�� ��߿��ƾ���
//���ƾ����ǿ�����ߵ�
//���ǵĿ��ƾ���Ϊʲô�ϲ�ȥ��  ���������Ѿ���С�� Ҳ��������

//У���к����� �ӽ�����  ��ɫ������
//   ADC           IO       TIM

//�������ϵ�һ�������
//һ����Ӹ�������ࣻ ��⵽���ʵ�λ�ú�֪ͨ���أ������ػ��GPS����
//���ϼ��� �õ�У�����GPS���� ���ϴ��͸����ظ���GPS���� �ٶ��㹻��
//��֮���������֮��Ҫͨ��  ��CAN

void GPS_REVEISE(void)
{
    if(SA1_OFF) 
        return;
    else
        return;
    //GPS_Revise(Pointfp64 position);
}

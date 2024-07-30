#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/input.h>
#include <string.h>
#include <math.h>

#define PI 3.14

double calculateAngle(double side1, double side2) {
    double angle;
    
    // 计算夹角
    angle = atan(side1 / side2) * 180 / M_PI;
    
    return angle;
}



int main()
{
	int fd;

	fd = open("/dev/input/touchscreen0", O_RDWR);
	if (fd == -1)
	{
		perror("open /dev/input/touchscreen0 ");
		return 0;
	}

	
	struct input_event ev;
	int x,y;
	int Bx, By;
	int Ex, Ey;
	int xflag = 0,yflag = 0;
	while(1)
	{
		read(fd, &ev, sizeof(ev));

		if (ev.type == EV_ABS && ev.code == ABS_X) 
		{
			x = ev.value;
			if(xflag == 0){
				xflag = 1;
				Bx = x;	
			}
				
		}
		else if (ev.type == EV_ABS && ev.code == ABS_Y) 
		{
			y = ev.value;
			if(yflag == 0){
				yflag = 1;
				By = y;	
			}
		}
		
		if (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value == 0){
			Ex = x;
			Ey = y;
			printf("Bx: %d\n", Bx);
			printf("Bx: %d\n", By);
			printf("Ex: %d\n", Ex);
			printf("Ey: %d\n", Ey);

			int delta_x = Ex - Bx;
			int delta_y = Ey - By;
			int distance_x = fabs(delta_x);
			int distance_y = fabs(delta_y);
            double distance = sqrt((double)distance_x * (double)distance_x + (double)distance_y * (double)distance_y);

			// printf("finger move out ...\n");


			double angle = calculateAngle((double)distance_x,(double)distance_y);



			if(delta_x < -10 && delta_y > 10){
                printf("   ^\n");
                printf("  /\n");
                printf(" /\n");
			    printf("/\n");
                printf("Move East-north,angle: %.2lf,distance:%.2lf\n",angle,distance);
			}else if(delta_x < -15 && delta_y < -15){
                printf("\\\n");
                printf(" \\\n");
                printf("  \\\n");
                printf("   v\n");
                printf("Move East-south,angle: %.2lf,distance:%.2lf\n",angle,distance);

			}else if(delta_x > 15 && delta_y > 15){
                printf("^\n");
                printf(" \\\n");
                printf("  \\\n");
                printf("    \\\n");
                printf("Move West-north,angle: %.2lf,distance:%.2lf\n",angle,distance);
			}else if(delta_x > 15 && delta_y < -15){
                printf("   /\n");
                printf("  /\n");
                printf(" /\n");
                printf("v\n");
                printf("Move West-south,angle: %.2lf,distance:%.2lf\n",angle,distance);
			}else if((delta_x > -15 && delta_x < 15) && delta_y > 15){
                printf("^\n");
                printf("|\n");
                printf("|\n");
                printf("|\n");
                printf("Move North,angle: %.2lf,distance:%.2lf\n",angle,distance);
			}else if((delta_x > -15 && delta_x < 15) && delta_y < -15){
                printf("|\n");
                printf("|\n");
                printf("|\n");
                printf("v\n");
                printf("Move South,angle: %.2lf,distance:%.2lf\n",angle,distance);
			}else if((delta_y > -15 && delta_y < 15) && delta_x < -15){
                printf("--->\n");
                printf("Move East,angle: %.2lf,distance:%.2lf\n",angle,distance);
			}else if((delta_y > -15 && delta_y < 15) && delta_x > 15){
                printf("<---\n");
                printf("Move East,angle: %.2lf,distance:%.2lf\n",angle,distance);
			}else{
			    printf("Not Move!\n");
			}
			Bx = 0;
			By = 0;
			xflag = 0;
			yflag = 0;
		}	
			
	}
	
	return 0;
}

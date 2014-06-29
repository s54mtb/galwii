/*********************************/
// Simple Kalman filter
static double Q = 0.000001;
static double R = 0.01;
static double P = 1, X = 0, K;
 
void kalman_measurementUpdate()
 {
 K = (P + Q) / (P + Q + R);
 P = R * (P + Q) / (R + P + Q);
 }
 
double kalman_update(double measurement)
 {
	 double result;
   kalman_measurementUpdate();
   result = X + (measurement - X) * K;
   X = result;
   return result;
 }
/*********************************/


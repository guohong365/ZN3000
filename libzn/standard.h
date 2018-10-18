#pragma once

/**计算标准值及标准值上下限
 * @param A 输入参数，A[4] 性别 >0 男，<=0 女. A[5] 年龄 A[6] 身高 A[7] 体重 A[8] 收缩压 A[9] 舒张压 A[10] 心率
 * @param B 输出，标准值
 * @param Bl 输出，标准值下限
 * @param Bh 输出，标准值上限
 */
void calculateStandard(const double *A,double * B, double *Bl, double * Bh );
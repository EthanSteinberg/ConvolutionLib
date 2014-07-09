package convolutionlib;

import java.util.Arrays;

public class JNIConvolution
{
    public static void main(String[] args) {
        double oneArr[] = {4,3,7,2,7,9,5,2,7}; 

        int dims = 2;
        int size=3;


        double twoArr[] = {6,3,7,2,8,4,2,7,5}; 

        System.out.println(Arrays.toString(performConvolutionInC(oneArr,twoArr,size,dims)));
    }

    public static native double[] performConvolutionInC(double[] a, double[] b, int size, int dims);

    static {
        System.loadLibrary("JNIConvolution");
    }
}

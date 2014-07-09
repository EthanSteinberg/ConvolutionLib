package convolutionlib;

import java.util.Arrays;

public class JNIConvolution
{
    public static void main(String[] args) {
        double oneArr[] = { 4,3,7, 0,0,0, 2,7,9, 0,0,0, 5,2,7, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
        }; 

        int dims = 2;
        int size = 6;


        double twoArr[] = { 6,3,7, 0,0,0, 2,8,4, 0,0,0, 2,7,5, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,

        }; 

        System.out.println(Arrays.toString(performConvolutionInC(oneArr,twoArr,size,dims)));
    }

    public static native double[] performConvolutionInC(double[] a, double[] b, int size, int dims);

    static {
        System.loadLibrary("JNIConvolution");
    }
}

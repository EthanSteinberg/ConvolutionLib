package convolutionlib;

import java.util.Arrays;
import java.util.Map;
import java.util.HashMap;

public class JNIConvolution
{
    

    final int dims;

    final Map<Integer,Long> internalConvolutionEngines = new HashMap<>();

    public JNIConvolution(int dims)
    {
        this.dims = dims;
    }

    public double[] convolute(double[] a, double[] b, int size)
    {
        int neededSize = (int) Math.pow(size,dims);

        if (a.length != neededSize || b.length != neededSize || a.length != b.length)
        {
            throw new RuntimeException("Fail with sizes " + neededSize + " " + a.length + " " + b.length);
        }

        long engine = internalConvolutionEngines.computeIfAbsent(size,notUsed->setupConvolution(size,dims));
        return performConvolution(engine,a,b);

    }

    public static void main(String[] args) {
        double oneArr[] = { 4,3,7, 0,0,0, 2,7,9, 0,0,0, 5,2,7, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,
        }; 

        int dims = 2;
        int size = 6;


        double twoArr[] = { 6,3,7, 0,0,0, 2,8,4, 0,0,0, 2,7,5, 0,0,0,
                            0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0, 0,0,0,

        }; 

        System.out.println(Arrays.toString(performConvolutionInC(oneArr,twoArr,size,dims)));

        JNIConvolution foo = new JNIConvolution(2);

        double result[] = foo.convolute(oneArr,twoArr,size);
        System.out.println(Arrays.toString(result));
    }

    public static native double[] performConvolutionInC(double[] a, double[] b, int size, int dims);

    private static native long setupConvolution(int size, int dims);
    private static native double[] performConvolution(long engine, double[] a, double[] b);

    static {
        System.loadLibrary("JNIConvolution");
    }
}

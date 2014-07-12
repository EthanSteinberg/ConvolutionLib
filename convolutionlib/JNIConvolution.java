package convolutionlib;

import java.util.Arrays;
import java.util.Map;
import java.util.HashMap;

public class JNIConvolution implements AutoCloseable
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

        if (!internalConvolutionEngines.containsKey(size))
            throw new RuntimeException("You need to prepare the engine for a certain size before convoluting.");
        
        long engine = internalConvolutionEngines.get(size);
        return performConvolution(engine,a,b);

    }


    public void prepareEngine(int size)
    {
        internalConvolutionEngines.put(size,setupConvolutionEngine(size,dims));
    }

    @Override
    public void close()
    {
        for (long engine : internalConvolutionEngines.values())
        {
            releaseConvolutionEngine(engine);
        }

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

        try (JNIConvolution foo = new JNIConvolution(dims))
        {
            foo.prepareEngine(size);
            double result[] = foo.convolute(oneArr,twoArr,size);
            System.out.println(Arrays.toString(result));
        }        
    }

    public static native double[] performConvolutionInC(double[] a, double[] b, int size, int dims);

    private static native long setupConvolutionEngine(int size, int dims);
    private static native double[] performConvolution(long engine, double[] a, double[] b);
    private static native void releaseConvolutionEngine(long engine);

    static {
        System.loadLibrary("JNIConvolution");
    }
}

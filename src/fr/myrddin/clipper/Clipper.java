package fr.myrddin.clipper;


public class Clipper
{
	public static final int INTERSECTION = 0;
	public static final int UNION = 1;
	public static final int DIFFERENCE = 2;
	public static final int XOR = 3;

	public static final int EVEN_ODD = 0;
	public static final int NON_ZERO = 1;

	static
	{
		System.loadLibrary("Clipper");
	}
	
	 private final long addr;
	
	public Clipper()
	{
		addr = jniNewClipper();
	}
	
	public void dispose()
	{
		jniDispose(addr);
	}
	
	public boolean addSubject(double[] subjectPolygon)
	{
		return jniAddSubject(addr,subjectPolygon);
	}
	
	public boolean addClip(double[] clipPolygon)
	{
		return jniAddClip(addr,clipPolygon);
	}
	
	public double[][] execute(int clipType)
	{
		return jniExecute(addr,clipType, EVEN_ODD, EVEN_ODD);
	}

	public double[][] clipPolygon(double[] subjectPolygon, double[] clipPolygon, int clipType)
	{
		return jniClipPolygon(subjectPolygon, clipPolygon, clipType, EVEN_ODD, EVEN_ODD);
	}

	private native long jniNewClipper();
	private native void jniDispose(long addr);
	private native boolean jniAddSubject(long addr, double[] subjectPolygon);
	private native boolean jniAddClip(long addr, double[] clipPolygon);
	private native double[][] jniExecute(long addr, int clipType, int subjectFillType, int clipFillType);
	private native double[][] jniClipPolygon(double[] subjectPolygon, double[] clipPolygon, int clipType, int subjectFillType, int clipFillType);
}

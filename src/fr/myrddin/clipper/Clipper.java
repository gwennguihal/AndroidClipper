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

	public void clipPolygon(int[] subjectPolygon, int[] clipPolygon, int clipType, int subjectFillType, int clipFillType)
	{
		jniClipPolygon(subjectPolygon, clipPolygon, clipType, subjectFillType, clipFillType);
	}

	public int[][] clipPolygon(int[] subjectPolygon, int[] clipPolygon, int clipType)
	{
		return jniClipPolygon(subjectPolygon, clipPolygon, clipType, EVEN_ODD, EVEN_ODD);
	}

	private native int[][] jniClipPolygon(int[] subjectPolygon, int[] clipPolygon, int clipType, int subjectFillType, int clipFillType);
}

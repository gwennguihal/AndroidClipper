package fr.myrddin.clipper;

import android.util.Log;

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

	public void clipPolygon(double[] subjectPolygon, double[] clipPolygon, int clipType, int subjectFillType, int clipFillType)
	{
		jniClipPolygon(subjectPolygon, clipPolygon, clipType, subjectFillType, clipFillType);
	}

	public double[][] clipPolygon(double[] subjectPolygon, double[] clipPolygon, int clipType)
	{
		return jniClipPolygon(subjectPolygon, clipPolygon, clipType, EVEN_ODD, EVEN_ODD);
	}

	private native double[][] jniClipPolygon(double[] subjectPolygon, double[] clipPolygon, int clipType, int subjectFillType, int clipFillType);
}

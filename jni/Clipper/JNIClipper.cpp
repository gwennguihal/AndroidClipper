/*******************************************************************************
 *                                                                              *
 * Author    :  Ari Þór H. Arnbjörnsson                                         *
 * Date      :  22 May 2011                                                     *
 * Website   :  http://flassari.is                                              *
 *                                                                              *
 * License:                                                                     *
 * Use, modification & distribution is subject to Boost Software License Ver 1. *
 * http://www.boost.org/LICENSE_1_0.txt                                         *
 *                                                                              *
 *******************************************************************************/

#include "clipper.hpp"
#include "Clipper.h"
#include "clipper.cpp"

#ifdef ANDROID
#include <android/log.h>
#endif

using namespace clipper;

JNIEXPORT jlong JNICALL Java_fr_myrddin_clipper_Clipper_jniNewClipper(JNIEnv *env, jobject obj) 
{
	Clipper* c = new Clipper();
	return (jlong)c;
}

JNIEXPORT void JNICALL Java_fr_myrddin_clipper_Clipper_jniDispose(JNIEnv *env, jobject obj, jlong addr) 
{
	Clipper* c = (Clipper*)addr;
	delete c;
}

JNIEXPORT jboolean JNICALL Java_fr_myrddin_clipper_Clipper_jniAddSubject(
		JNIEnv *env, jobject obj, jlong addr, jdoubleArray subjectPolygonArray) {

	Clipper* c = (Clipper*)addr;
	
	jint subjectPolygonCount; 
	subjectPolygonCount = env->GetArrayLength(subjectPolygonArray);
	Polygon subjectPolygon(subjectPolygonCount / 2);
	jdouble buf[subjectPolygonCount];
	jint i;
	env->GetDoubleArrayRegion(subjectPolygonArray, 0, subjectPolygonCount, buf);
	for (i = 0; i < subjectPolygonCount; i += 2) {
		subjectPolygon[i / 2] = IntPoint((int) buf[i], (int) buf[i + 1]);
	}

	return c->AddPolygon(subjectPolygon, ptSubject);
}

JNIEXPORT jboolean JNICALL Java_fr_myrddin_clipper_Clipper_jniAddClip(
		JNIEnv *env, jobject obj, jlong addr, jdoubleArray clipPolygonArray) {

	Clipper* c = (Clipper*)addr;
	
	jint clipPolygonCount;
	clipPolygonCount = env->GetArrayLength(clipPolygonArray);
	Polygon clipPolygon(clipPolygonCount / 2);
	jdouble buf2[clipPolygonCount];
	jint i;
	env->GetDoubleArrayRegion(clipPolygonArray, 0, clipPolygonCount, buf2);
	for (i = 0; i < clipPolygonCount; i += 2) {
		clipPolygon[i / 2] = IntPoint( (int) buf2[i], (int) buf2[i + 1]);
	}

	return c->AddPolygon(clipPolygon, ptClip);
}

JNIEXPORT jobjectArray JNICALL Java_fr_myrddin_clipper_Clipper_jniExecute(
		JNIEnv *env, jobject obj, jlong addr, jint clipTypeArg, jint subjectFillTypeArg, jint clipFillTypeArg) {
	
	Clipper* c = (Clipper*)addr;
	
	Polygons solution;

	ClipType clipType;
	switch (clipTypeArg) {
	default:
	case 0:
		clipType = ctIntersection;
		break;
	case 1:
		clipType = ctUnion;
		break;
	case 2:
		clipType = ctDifference;
		break;
	case 3:
		clipType = ctXor;
		break;
	}

	PolyFillType subjectFillType, clipFillType;
	
	switch (subjectFillTypeArg) {
	default:
	case 0:
		subjectFillType = pftEvenOdd;
		break;
	case 1:
		subjectFillType = pftNonZero;
		break;
	}
	
	switch (clipFillTypeArg) {
	default:
	case 0:
		clipFillType = pftEvenOdd;
		break;
	case 1:
		clipFillType = pftNonZero;
		break;
	}

	if (c->Execute(clipType, solution, subjectFillType, clipFillType)) 
	{

		jobjectArray result;
		jclass intArrCls = env->FindClass("[[D");
		if (intArrCls == NULL) {
			return NULL;
		}
		result = env->NewObjectArray((int) solution.size(), intArrCls,NULL);
		
		for (int i = 0; i < (int) solution.size(); i++) 
		{

			jdoubleArray poly;
			poly = env->NewDoubleArray((int) solution[i].size() * 2);

			if (poly == NULL) {
				return NULL; // out of memory error thrown
			}

			jdouble fill[(int) solution[i].size() * 2];
			for (int j = 0; j < (int) solution[i].size(); j++) {
				// Push all the vertices into the array
				fill[j * 2] = solution[i][j].X; // put whatever logic you want to populate the values here.
				fill[j * 2 + 1] = solution[i][j].Y;
			}
			// Insert the array into the returnArray
			env->SetDoubleArrayRegion(poly, 0, (int) solution[i].size() * 2,fill);
			
			env->SetObjectArrayElement(result, i, poly);
         	env->DeleteLocalRef(poly);
		}
		return result;
	}
	return NULL;
}


JNIEXPORT jobjectArray JNICALL Java_fr_myrddin_clipper_Clipper_jniClipPolygon(
		JNIEnv *env, jobject obj, jdoubleArray subjectPolygonArray, jdoubleArray clipPolygonArray, jint clipTypeArg, jint subjectFillTypeArg,
		jint clipFillTypeArg) {

	jint subjectPolygonCount; 
	subjectPolygonCount = env->GetArrayLength(subjectPolygonArray);
	
	jint clipPolygonCount;
	clipPolygonCount = env->GetArrayLength(clipPolygonArray);
	
	Polygon subjectPolygon(subjectPolygonCount / 2), clipPolygon(
			clipPolygonCount / 2);
	Polygons solution;

	jdouble buf[subjectPolygonCount];
	jint i;
	env->GetDoubleArrayRegion(subjectPolygonArray, 0, subjectPolygonCount, buf);
	for (i = 0; i < subjectPolygonCount; i += 2) {
		subjectPolygon[i / 2] = IntPoint((int) buf[i], (int) buf[i + 1]);
	}

	//	// Populate the clip polygon
	jdouble buf2[clipPolygonCount];
	env->GetDoubleArrayRegion(clipPolygonArray, 0, clipPolygonCount, buf2);
	for (i = 0; i < clipPolygonCount; i += 2) {
		clipPolygon[i / 2] = IntPoint( (int) buf2[i], (int) buf2[i + 1]);
	}

	ClipType clipType;
	switch (clipTypeArg) {
	default:
	case 0:
		clipType = ctIntersection;
		break;
	case 1:
		clipType = ctUnion;
		break;
	case 2:
		clipType = ctDifference;
		break;
	case 3:
		clipType = ctXor;
		break;
	}

	PolyFillType subjectFillType, clipFillType;
	switch (subjectFillTypeArg) {
	default:
	case 0:
		subjectFillType = pftEvenOdd;
		break;
	case 1:
		subjectFillType = pftNonZero;
		break;
	}
	switch (clipFillTypeArg) {
	default:
	case 0:
		clipFillType = pftEvenOdd;
		break;
	case 1:
		clipFillType = pftNonZero;
		break;
	}

	Clipper c;
	c.AddPolygon(subjectPolygon, ptSubject);
	c.AddPolygon(clipPolygon, ptClip);

	if (c.Execute(clipType, solution, subjectFillType, clipFillType)) {

		jobjectArray result;
		jclass intArrCls = env->FindClass("[[D");
		if (intArrCls == NULL) {
			return NULL;
		}
		result = env->NewObjectArray((int) solution.size(), intArrCls,NULL);
		
		for (int i = 0; i < (int) solution.size(); i++) 
		{

			jdoubleArray poly;
			poly = env->NewDoubleArray((int) solution[i].size() * 2);

			if (poly == NULL) {
				return NULL; // out of memory error thrown
			}

			jdouble fill[(int) solution[i].size() * 2];
			for (int j = 0; j < (int) solution[i].size(); j++) {
				// Push all the vertices into the array
				fill[j * 2] = solution[i][j].X; // put whatever logic you want to populate the values here.
				fill[j * 2 + 1] = solution[i][j].Y;
			}
			// Insert the array into the returnArray
			env->SetDoubleArrayRegion(poly, 0, (int) solution[i].size() * 2,fill);
			
			env->SetObjectArrayElement(result, i, poly);
         	env->DeleteLocalRef(poly);
		}
		return result;
	}
	return NULL;
}

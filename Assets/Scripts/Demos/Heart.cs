using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Heart : GenericBehaviour
{
    float phase = 0;
    int lengthPoints;
    GameObject[,] points;
    Material[] materials;
    float DistanceBetweenPoint = 0.05f;
    float scalePoints = 0.5f;

    // Start is called before the first frame update
    public override void Constructor()
    {
        lengthPoints = (int)((Mathf.PI * 2) / DistanceBetweenPoint) + 1;

        points = new GameObject[3,lengthPoints];
        materials = new Material[lengthPoints];

        for (int i = 0; i < lengthPoints; i++)
        {
            materials[i] = new Material(Shader.Find("UI/Default"));
            materials[i].color = Color.white;
            materials[i].renderQueue = (int)UnityEngine.Rendering.RenderQueue.Transparent;
        }

        for (int j = 0; j < 3; j++)
            for (int i = 0; i < lengthPoints; i++)
            {
                points[j, i] = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                points[j, i].transform.parent = gameObject.transform;
                points[j, i].name = i.ToString();
                points[j, i].transform.localScale = new Vector3(scalePoints, scalePoints, scalePoints);

                points[j, i].GetComponent<MeshRenderer>().material = materials[i];
            }
    }

    // Update is called once per frame
    public override void Generate()
    {
    }

    private void OnDrawGizmos()
    {

        SimpleHearth2();
    }

    void SimpleHearth()
    {

        float x, y;
        int k = 0;
        for (float i = 0; i < Mathf.PI * 2; i += DistanceBetweenPoint)
        {
            float transparancy = (i + phase) % 1;

            points[0,k].GetComponent<MeshRenderer>().material.color = new Color(0, 0, 0, transparancy);

            float t = i + phase;

            x = 16 * Mathf.Pow(Mathf.Sin(t), 3);
            y = 13 * Mathf.Cos(t) - 5 * Mathf.Cos(2 * t) - 2 * Mathf.Cos(3 * t) - Mathf.Cos(4 * t);

            points[0,k].transform.position = new Vector3(x, y, 0);

            k++;
        }

        phase += 0.001f;
    }

    void SimpleHearthZooming()
    {

        float xHearth, yHearth;
        int k = 0;
        for (float i = 0; i < Mathf.PI * 2; i += DistanceBetweenPoint)
        {
            float transparancy = (i + phase) % 1;
            points[0,k].GetComponent<MeshRenderer>().material.color = new Color(0, 0, 0, transparancy);
            float t = i;

            xHearth = 16 * Mathf.Pow(Mathf.Sin(t), 3);
            yHearth = 13 * Mathf.Cos(t) - 5 * Mathf.Cos(2 * t) - 2 * Mathf.Cos(3 * t) - Mathf.Cos(4 * t);

            double noise = PGApi.PGApi.PerlinNoiseUnity(phase);

            xHearth *= (float)noise;
            yHearth *= (float)noise;

            points[0,k].transform.position = new Vector3(xHearth, yHearth, 0);
            k++;
        }

        phase += 0.001f;
    }

    void SimpleHearth2()
    {
        float xHearth, yHearth;
        for (int j = 0; j < 3; j++)
        {
            int k = 0;
            for (float i = 0; i < Mathf.PI * 2; i += DistanceBetweenPoint)
            {
                float t = i + phase;
                float scale = (float)ScaleValue(PGApi.PGApi.PerlinNoiseUnity(t), 0, 1, 0.01f, scalePoints);
                points[j,k].transform.localScale = new Vector3(scale, scale, scale);

                float transparancy = (t+j) % 1;
                points[j,k].GetComponent<MeshRenderer>().material.color = new Color(0, 0, 0, transparancy);

                xHearth = HearthX(t);
                yHearth = HearthY(t);

                double noise = ScaleValue(PGApi.PGApi.PerlinNoise2DUnity(
                    Mathf.Cos(i) + 1 + j/5,
                    Mathf.Sin(i) + 1 + j/5), 0, 1, 0.9f-j/20f, 1.1f-j/20f);

                xHearth *= (float)noise;
                yHearth *= (float)noise;

                points[j, k].transform.position = new Vector3(xHearth - j / 20f, yHearth - j / 20f, 0);
                k++;
            }
        }

        phase += 0.001f;
    }

    double ScaleValue(double value,
              double originalMin, double originalMax,
              double newMin, double newMax)
    {
        // scale original value to range 0...1
        double normalizedValue = (value - originalMin)/ (originalMax - originalMin);

        double lerpedValue = newMin + normalizedValue * (newMax - newMin);

        // return new value in new range
        return lerpedValue;
    }

    float HearthX(float x)
    {
        return 16 * Mathf.Pow(Mathf.Sin(x), 3);
    }

    float HearthY(float x)
    {
        return 13 * Mathf.Cos(x) - 5 * Mathf.Cos(2 * x) - 2 * Mathf.Cos(3 * x) - Mathf.Cos(4 * x);
    }
}

﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Heart3 : GenericBehaviour
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

        points = new GameObject[3, lengthPoints];
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
        SimpleHearthZooming();
    }

    void SimpleHearthZooming()
    {

        float xHearth, yHearth;
        int k = 0;
        for (float i = 0; i < Mathf.PI * 2; i += DistanceBetweenPoint)
        {
            float transparancy = (i + phase) % 1;
            points[0, k].GetComponent<MeshRenderer>().material.color = new Color(0, 0, 0, transparancy);
            float t = i;

            xHearth = 16 * Mathf.Pow(Mathf.Sin(t), 3);
            yHearth = 13 * Mathf.Cos(t) - 5 * Mathf.Cos(2 * t) - 2 * Mathf.Cos(3 * t) - Mathf.Cos(4 * t);

            float noise = PGApi.PGApi.ZgomotulPerlin_Îmbunătățit1D(phase);

            xHearth *= (float)noise;
            yHearth *= (float)noise;

            points[0, k].transform.position = new Vector3(xHearth, yHearth, 0);
            k++;
        }

        phase += 0.001f;
    }
}

﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// MESH GENERATION in Unity - Basics : https://youtu.be/eJEpeUH1EMg?list=WL
// PROCEDURAL TERRAIN in Unity! - Mesh Generation : https://youtu.be/64NblGkAabk?list=WL

[RequireComponent(typeof(MeshFilter))]
public class MeshGenerator : MonoBehaviour
{
    private GameObject terrain;
    private Mesh mesh;
    private Vector3[] vertices;
    private int[] triangles;

    public int xSize = 20;
    public int zSize = 20;

    public int xOffSet = 0;
    public int zOffSet = 0;

    public GameObject parent;

    // Start is called before the first frame update
    public void Start()
    {
        terrain = new GameObject("Terrain");
        mesh = new Mesh();
        terrain.AddComponent<MeshFilter>().mesh = mesh;
        MeshRenderer meshRenderer = terrain.AddComponent<MeshRenderer>();
        meshRenderer.material = new Material(Shader.Find("Specular"));

        CreateShape();
        UpdateShape();

        parent = gameObject;
        if(parent)
            terrain.transform.SetParent(parent.transform);
    }

    public void GenerateNewMesh()
    {
        xOffSet = Random.Range(0, 1000);
        zOffSet = Random.Range(0, 1000);

        for (int i = 0, z = 0; z <= zSize; z++)
        {
            for (int x = 0; x <= xSize; x++)
            {
                float y = Mathf.PerlinNoise(xOffSet + x * .3f, zOffSet + z * .3f) * 2f;
                vertices[i] = new Vector3(x, y, z);
                i++;
            }
        }

        UpdateShape();
    }

    //private void Update()
    //{
    //    //reload scene, for testing purposes
    //    if (Input.GetKeyDown("r"))
    //        {
    //            GenerateNewMesh();
    //        }
    //}


    private void CreateShape()
    {
        vertices = new Vector3[(xSize + 1) * (zSize + 1)];

        for (int i = 0, z = 0; z <= zSize; z++)
        {
            for (int x = 0; x <= xSize; x++)
            {
                float y = Mathf.PerlinNoise(xOffSet + x * .3f,zOffSet + z * .3f) * 2f;
                vertices[i] = new Vector3(x, y, z);
                i++;
            }
        }

        triangles = new int[6 * xSize * zSize];
        int vert = 0;
        int tris = 0;
        for (int z = 0; z < zSize; z++)
        {
            for (int x = 0; x < xSize; x++)
            {
                triangles[tris + 0] = vert + 0;
                triangles[tris + 1] = vert + xSize + 1;
                triangles[tris + 2] = vert + 1;
                triangles[tris + 3] = vert + 1;
                triangles[tris + 4] = vert + xSize + 1;
                triangles[tris + 5] = vert + xSize + 2;

                vert++;
                tris += 6;
            }
            vert++;
        }
    }

    private void UpdateShape()
    {
        mesh.Clear();
        mesh.vertices = vertices;
        mesh.triangles = triangles;
    }

    private void OnDrawGizmos()
    {
        if (vertices == null)
            return;

        for (int i = 0; i < vertices.Length; i++)
        {
            Gizmos.DrawSphere(vertices[i], 0.1f);
        }
    }
}
using System.Runtime.InteropServices;
using UnityEngine;

[RequireComponent(typeof(MeshFilter))]
public class MeshGenerator : GenericBehaviour
{
    private GameObject terrain;
    private Mesh mesh;
    private Vector3[] vertices;
    private int[] triangles;
    private Color[] colors;

    public int xSize = 20;
    public int zSize = 20;

    public int xOffSet = 0;
    public int zOffSet = 0;

    public GameObject parent;

    [DllImport("PG_Library", CallingConvention = CallingConvention.Cdecl)]
    private static extern double PerlinNoise2DUnity(double x, double y);

    // Start is called before the first frame update
    public override void Start()
    {
        if (terrain != null)
        {
            return;
        }

        terrain = new GameObject("Terrain");
        mesh = new Mesh();

        terrain.AddComponent<MeshFilter>().mesh = mesh;
        MeshRenderer meshRenderer = terrain.AddComponent<MeshRenderer>();
        meshRenderer.material = new Material(Shader.Find("Legacy Shaders/Particles/Additive"));

        CreateShape();

        parent = gameObject;
        if (parent)
        {
            terrain.transform.SetParent(parent.transform);
        }
    }

    public override void Generate()
    {
        xOffSet = Random.Range(0, 1000);
        zOffSet = Random.Range(0, 1000);

        for (int i = 0, z = 0; z <= zSize; z++)
        {
            for (int x = 0; x <= xSize; x++)
            {
                float y = (float)PerlinNoise2DUnity(xOffSet + (x * .3f), zOffSet + (z * .3f)) * 2f;
                vertices[i] = new Vector3(x, y, z);
                float colorValue = (float)PerlinNoise2DUnity(xOffSet + x * .3f, zOffSet + z * .3f);
                colors[i] = Color.Lerp(Color.red, Color.green, colorValue);
                i++;
            }
        }

        UpdateShape();
    }

    private void CreateShape()
    {
        vertices = new Vector3[(xSize + 1) * (zSize + 1)];
        colors = new Color[(xSize + 1) * (zSize + 1)];

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
        mesh.colors = colors;
    }

    //private void OnDrawGizmos()
    //{
    //    if (vertices == null)
    //        return;
    //
    //    for (int i = 0; i < vertices.Length; i++)
    //    {
    //        Gizmos.DrawSphere(vertices[i], 0.1f);
    //    }
    //}
}
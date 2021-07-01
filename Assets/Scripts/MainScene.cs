using System.Collections.Generic;
using UnityEngine;

public class MainScene : MonoBehaviour
{
    #region Members
    private GameObject m_CameraObject = null;
    private GameObject m_UI = null;
    private PreviewDemos m_PreviewDemos;
       
    public GameObject parentCanvas = null;
    public GameObject buttonPrefab = null;
    #endregion

    #region Methods

    // Start is called before the first frame update
    void Start()
    {
        m_PreviewDemos = new PreviewDemos(parentCanvas,buttonPrefab);

        m_CameraObject = new GameObject("MainCamera");
        m_CameraObject.transform.position = new Vector3(0, 0, -10);

        Camera m_MainCamera = m_CameraObject.AddComponent<Camera>();
        m_MainCamera.allowMSAA = false;
        m_MainCamera.allowHDR = false;
        m_MainCamera.allowDynamicResolution = false;
        m_MainCamera.clearFlags = CameraClearFlags.SolidColor;
        m_MainCamera.backgroundColor = new Color32(121, 49, 49, 0);
        m_MainCamera.orthographic = true;
        m_MainCamera.useOcclusionCulling = false;
        m_MainCamera.cullingMask = LayerMask.GetMask("Default");

        m_MainCamera.transform.position = new Vector3(0, 0, -10f);
        m_MainCamera.transform.eulerAngles = new Vector3(0, 0, 0);
        m_MainCamera.orthographicSize = 44.59f;
        m_MainCamera.farClipPlane = 300;
        m_MainCamera.nearClipPlane = 0.3f;

        m_UI = new GameObject("UI");
        m_UI.transform.localPosition = new Vector3(0, 45, -9f);
        m_UI.transform.localScale = new Vector3(1, 1, 1);

        GameObject pressR = new GameObject("Press R to reload");
        pressR.transform.SetParent(m_UI.transform);
        pressR.transform.localPosition = new Vector3(0, 0, 0);

        //TextMesh text = pressR.AddComponent<TextMesh>();
        //text.text = "Press R to reload";
        //text.offsetZ = 0;
        //text.characterSize = 5;
        //text.lineSpacing = 1;
        //text.anchor = TextAnchor.UpperCenter;
        //text.alignment = TextAlignment.Center;
        //text.tabSize = 4;
        //text.fontSize = 0;
        //text.fontStyle = FontStyle.Normal;
        //text.richText = true;
        //text.font = null;
        //text.color = Color.white;

        m_PreviewDemos.AddNewDemo<LevelGeneration>("Room Generator");                 

        GameObject demo_Level = m_PreviewDemos.AddNewDemo<Atestat>("Atestat");        
        demo_Level.transform.localScale = new Vector3(6.6f, 6.6f, 1);

        demo_Level = m_PreviewDemos.AddNewDemo<MeshGenerator>("Mesh Generator");      
        demo_Level.transform.localPosition = new Vector3(-60, -38, 5);
        demo_Level.transform.localEulerAngles = new Vector3(-30f, -1, -5);
        demo_Level.transform.localScale = new Vector3(7, 7, 7);

        m_PreviewDemos.AddNewDemo<MDS>("MDS");                                        

        demo_Level = m_PreviewDemos.AddNewDemo<PerlinNoisePreview>("Perlin Preview"); 
        //demo_Level.transform.localScale = new Vector3(88, 88, 1);

        m_PreviewDemos.AddNewDemo<Heart>("Hearth Preview");
        m_PreviewDemos.AddNewDemo<Heart1>("Hearth Preview 1");
        m_PreviewDemos.AddNewDemo<NoisesDemo>("Perlin Noises");                       
        m_PreviewDemos.AddNewDemo<MarbleTexture>("Marble Texture");                   
        demo_Level = m_PreviewDemos.AddNewDemo<PerlinTexture>("Perlin Textures");     
        demo_Level.transform.localScale = new Vector3(1, 1, 1);

        m_PreviewDemos.AddNewDemo<EarthMap>("Earth Map");
        m_PreviewDemos.AddNewDemo<Marble1>("Marble 1");
        m_PreviewDemos.AddNewDemo<WoodTexture>("Wood Texture");
        m_PreviewDemos.AddNewDemo<CloudTexture>("Cloud Texture");
        m_PreviewDemos.AddNewDemo<FurTexture>("Fur Texture");

    }

    // Update is called once per frame
    void Update()
    {
        m_PreviewDemos.UpdateOnKeyPress();

        if (Input.GetKeyDown("d"))
        {
            m_UI.SetActive(!m_UI.activeSelf);
            return;
        }
    }

    #endregion
}
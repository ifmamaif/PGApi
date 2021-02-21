using System.Collections.Generic;
using UnityEngine;

public class MainScene : MonoBehaviour
{
    #region Members
    GameObject m_CameraObject = null;
    GameObject m_UI = null;

    private delegate void ResetLevelFunction();
    private ResetLevelFunction resetLevel;

    readonly List<KeyValuePair<GameObject, ResetLevelFunction>> listOfDemos = new List<KeyValuePair<GameObject, ResetLevelFunction>>();

    #endregion

    #region Methods

    // Start is called before the first frame update
    void Start()
    {
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

        TextMesh text = pressR.AddComponent<TextMesh>();
        text.text = "Press R to reload";
        text.offsetZ = 0;
        text.characterSize = 5;
        text.lineSpacing = 1;
        text.anchor = TextAnchor.UpperCenter;
        text.alignment = TextAlignment.Center;
        text.tabSize = 4;
        text.fontSize = 0;
        text.fontStyle = FontStyle.Normal;
        text.richText = true;
        text.font = null;
        text.color = Color.white;

        GameObject changeGenerator = new GameObject("Change Generator");
        changeGenerator.transform.SetParent(m_UI.transform);
        changeGenerator.transform.localPosition = new Vector3(-108, -44, 0);

        text = changeGenerator.AddComponent<TextMesh>();
        text.text = "Press 1\nPress 2\nPress 3\nPress 4\nPress 0";
        text.offsetZ = 0;
        text.characterSize = 8;
        text.lineSpacing = 1;
        text.anchor = TextAnchor.MiddleLeft;
        text.alignment = TextAlignment.Center;
        text.tabSize = 4;
        text.fontSize = 0;
        text.fontStyle = FontStyle.Normal;
        text.richText = true;
        text.font = null;
        text.color = Color.black;

        NewDemo<LevelGeneration>("Room Generator");

        GameObject demo_Level = NewDemo<Atestat>("Atestat");
        demo_Level.transform.localScale = new Vector3(6.6f, 6.6f, 1);

        demo_Level = NewDemo<MeshGenerator>("Mesh Generator");
        demo_Level.transform.localPosition = new Vector3(-60, -38, 5);
        demo_Level.transform.localEulerAngles = new Vector3(-30f, -1, -5);
        demo_Level.transform.localScale = new Vector3(7, 7, 7);

        NewDemo<MDS>("MDS");

        demo_Level = NewDemo<PerlinNoisePreview>("Perlin Preview");
        demo_Level.transform.localScale = new Vector3(88, 88, 1);

        ActivateDemo(listOfDemos[0].Key);
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKeyDown("r"))
        {
            resetLevel();
            return;
        }
        if (Input.GetKeyDown("d"))
        {
            m_UI.SetActive(!m_UI.activeSelf);
            return;
        }

        for (int i = 0; i < 10 && i < listOfDemos.Count; i++)
        {
            if (Input.GetKeyDown(i.ToString()))
            {
                ActivateDemo(listOfDemos[i].Key);
                return;
            }
        }
    }

    GameObject NewDemo<T>(string Name) where T : GenericBehaviour
    {
        GameObject newObject = new GameObject(Name);
        var newComponentScript = newObject.AddComponent<T>();

        listOfDemos.Add(new KeyValuePair<GameObject, ResetLevelFunction>(newObject, () => { newComponentScript.Generate(); }));
        newComponentScript.Start();

        return newObject;
    }

    void ActivateDemo(GameObject gameObject)
    {
        listOfDemos.ForEach(x => x.Key.SetActive(false));
        var pair = listOfDemos.Find(x => x.Key.name == gameObject.name);
        resetLevel = pair.Value;
        resetLevel();
        pair.Key.SetActive(true);
    }

    #endregion
}
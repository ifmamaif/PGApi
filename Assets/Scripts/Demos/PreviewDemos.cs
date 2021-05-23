using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using UnityEngine;
using UnityEngine.UI;

class PreviewDemos
{
    private delegate void ResetLevelFunction();
    private ResetLevelFunction resetLevel;

    private readonly List<KeyValuePair<GameObject, ResetLevelFunction>> m_ListOfDemos = new List<KeyValuePair<GameObject, ResetLevelFunction>>();
    private readonly List<GameObject> m_Buttons = new List<GameObject>();
    private readonly GameObject m_PrefabButton;
    private readonly GameObject m_ParentCanvas;
    private GameObject m_CurrentDemo = null;
    private const int OFFSET_BUTTONS_Y = 300;

    public PreviewDemos(GameObject parentCanvas,GameObject prefabButton)
    {
        m_ParentCanvas = parentCanvas;
        m_PrefabButton = prefabButton;
    }

    public void UpdateOnKeyPress()
    {
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if(m_CurrentDemo != null)
            {
                m_CurrentDemo.SetActive(false);
                m_CurrentDemo = null;
                m_Buttons.ForEach(x => x.SetActive(true));
            }
        }

        if (Input.GetKeyDown("r"))
        {
            if (m_CurrentDemo != null)
            {
                resetLevel();
            }
        }
    }

    public GameObject AddNewDemo<T>(string Name) where T : GenericBehaviour
    {
        GameObject newObject = new GameObject(Name);
        newObject.SetActive(false);
        var newComponentScript = newObject.AddComponent<T>();

        ResetLevelFunction resetLevelFunction = () => newComponentScript.Generate();
        m_ListOfDemos.Add(new KeyValuePair<GameObject, ResetLevelFunction>(newObject, resetLevelFunction));
        newComponentScript.Constructor();

        var buttonObject= UnityEngine.Object.Instantiate(m_PrefabButton, new Vector3(0, 0, 0), Quaternion.identity, m_ParentCanvas.transform);
        buttonObject.transform.localPosition = new Vector3(0, OFFSET_BUTTONS_Y - m_Buttons.Count * 30, 0);
        m_Buttons.Add(buttonObject);

        var button = buttonObject.GetComponent<Button>();
        button.onClick.AddListener(() => ActivateDemo(newObject, resetLevelFunction));
        button.name = Name;

        var textButton = buttonObject.transform.GetChild(0).gameObject.GetComponent<Text>();
        textButton.text = Name;

        
        return newObject;
    }

    void ActivateDemo(GameObject gameObject, ResetLevelFunction resetLevelFunction)
    {
        m_Buttons.ForEach(x => x.SetActive(false));
        m_CurrentDemo = gameObject;

        resetLevel = resetLevelFunction;
        resetLevel();
        gameObject.SetActive(true);
    }
}
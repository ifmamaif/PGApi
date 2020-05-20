using UnityEngine;

//public interface BaseBehaviour
//{
//    public abstract void Start();

//    public abstract void Generate();
//}

public class GenericBehaviour : MonoBehaviour
{
    public virtual void Start() { }

    public virtual void Generate() { }
}
using UnityEngine;
using UnityEngine.UI;
using System.Collections;
using System.Collections.Generic;

public class PlayerCamera : MonoBehaviour {

    public float smoothing = 5f;
    Transform target;

    Vector3 offset;
    Canvas HUDCanvas;

    List<GameObject> UIElements;

	void Awake () {
        HUDCanvas = GetComponent<Canvas>();
        offset = transform.position - target.position;
	}

    void FixedUpdate()
    {
        Vector3 targetCamPos = target.position + offset;
        transform.position = Vector3.Lerp(transform.position, targetCamPos, smoothing * Time.deltaTime);
    }

    public void RemoveUIElement(GameObject element)
    {
        if (UIElements.IndexOf(element) != -1)
        {
            UIElements.Remove(element);

            /*
             * If there is a lot of elements, a condition can be added to
             * break the for if the element has been found
             */
            for(int i = 0 ; i < HUDCanvas.transform.childCount ; i++){
                if (HUDCanvas.transform.GetChild(i) == element.transform) Destroy(HUDCanvas.transform.GetChild(i));
            }
        }
    }

    public void AddUIElement(GameObject element)
    {
        if (UIElements.IndexOf(element) == -1)
        {
            UIElements.Add(element);
            element.transform.parent = HUDCanvas.transform;
            element.transform.SetSiblingIndex(UIElements.Count-1);
        }
    }

    public Vector3 Target { get; set; }
}

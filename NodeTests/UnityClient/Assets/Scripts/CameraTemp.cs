//using Mirror;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraTemp : MonoBehaviour
{
    [SerializeField]
    Vector3 rotCenter = new Vector3(0f, 0f, 0f);

    float mouseSensetivity = 100f;
    float scrollSensetivity = 1.5f;
    
    [SerializeField]
    float roh = 15f;

    float prevPhi = 0f;
    float prevTheta= 0f;

    float posXS = 0f;
    float posYS = 5f;

    public GameObject camObj;

    Camera cam;

    //Manager1 manager;

    
    public Transform Drone;
    GameObject selectedPiece;

    void Start()
    {
        cam = transform.GetComponentInChildren<Camera>();
        // if(!isLocalPlayer) {
        //     camObj.gameObject.SetActive(false);
        // }
         Cursor.lockState = CursorLockMode.Locked;
        // StartCoroutine(MoveState());
        // manager = FindObjectOfType<Manager1>();

    }
    void Move() {

        //if(!isLocalPlayer) return;
        
        //Save mouse delta
        float mouseX = Input.GetAxis("Mouse X") * mouseSensetivity * Time.deltaTime;
        float mouseY = Input.GetAxis("Mouse Y") * mouseSensetivity * Time.deltaTime;
       
            //set circumference position
            posXS += mouseX;
            posYS += mouseY;
        
        //clamp circumference position in meters
        posYS = Mathf.Clamp(posYS, 1f, 11f); 
        //circumference position to angle in radians
        float phi = posYS / (2f * Mathf.PI);
        float theta = posXS / (2f * Mathf.PI);
        //Change radius 
        roh += Input.mouseScrollDelta.y * scrollSensetivity;
        roh = Mathf.Clamp(roh, 6f, 25f); 
        //Lerp phi and theta to smooth position / rotation if mouse is not clicked
        phi = Mathf.Lerp(prevPhi, phi, 10f * Time.deltaTime);
        theta = Mathf.Lerp(prevTheta, theta, 10f * Time.deltaTime); 

        //convert spherical coordinates to cartesian
        float posZ = Mathf.Sin(-phi) * Mathf.Sin(theta) * roh;
        float posY = Mathf.Cos(-phi) * roh;
        float posX = Mathf.Sin(-phi) * Mathf.Cos(theta) * roh;
        //set position and rotation
        transform.position = new Vector3(posX, posY, posZ) + rotCenter;
        transform.rotation = Quaternion.Euler(-(phi * Mathf.Rad2Deg) + 90f, -(theta * Mathf.Rad2Deg) + 90f, 0f);
        //set prev theta and phi
        prevTheta = theta;
        prevPhi = phi;
        
    }
    void CenterCamera() {
        rotCenter = Drone.position;
    }

    // bool Select() {
    //     Ray ray = cam.ScreenPointToRay(Input.mousePosition);

    //     if(Physics.Raycast(ray, out RaycastHit hitInf)) {
    //         //hitInf.transform.gameObject
    //         SelectPiece piece = hitInf.transform.gameObject.GetComponent<SelectPiece>();
    //         if(piece != null) {
    //             if(piece.canSelect){
    //                 return true;
    //             }
    //         }
    //     }
    //     return false;
    // }

    // GameObject PieceFromRaycast() {
    //     Ray ray = cam.ScreenPointToRay(Input.mousePosition);

    //     if(Physics.Raycast(ray, out RaycastHit hitInf)) {
    //         GameObject piece = hitInf.transform.gameObject;
    //         if(piece.GetComponent<SelectPiece>() != null) 
    //             return piece;
            
    //     }
    //     return null;
    // }

    // void SelectVisuals() {
    //     GameObject piece = PieceFromRaycast();

    //     ClearVisuals();

    //     if(piece == null) return;

    //     Renderer pieceRenderer = piece.GetComponent<Renderer>();

    //     if(piece.GetComponent<SelectPiece>().canSelect) 
    //         pieceRenderer.material.color = Color.green;
    //     else
    //         pieceRenderer.material.color = Color.red;
    // }

    // void ClearVisuals() {
    //     foreach(GameObject p in manager.blocks) {
    //         Renderer rend = p.GetComponent<Renderer>();
    //         rend.material = opaque;
    //         rend.material.color = Color.white;
            
    //     }
    // }

    // void TranslucentVisuals() {
    //     ClearVisuals();
    //     foreach(GameObject p in manager.blocks) {
    //         Renderer rend = p.GetComponent<Renderer>();
    //         rend.material = translucent;
    //         rend.material.SetColor("_Color",new Color(1f,1f,1f,.8f));

    //         //p.GetComponent<Renderer>().material.color = Color.clear;
    //     }
    //     Renderer render = selectedPiece.GetComponent<Renderer>();
    //     render.material = opaque;
    //     render.material.color = Color.green;
    // }

    void Update() {
        CenterCamera();
        Move();
    }
}
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class DroneMovement : MonoBehaviour
{
    private CharacterController controller;
    //private Vector3 playerVelocity;
    
    private float playerSpeed = 2.0f;
    [Range(0.0f, 1000.0f)] [SerializeField]
    private float thrust = 1000.0f;
    private float gravityValue = -9.81f;
    private Vector3 jh;
    private Vector3 gravVect;
    private Rigidbody rb;
    [Range(0.0f, 1000.0f)]
    public float clampVal = 1000.0f;


    private void Start()
    {
        //controller = gameObject.AddComponent<CharacterController>();
        rb = GetComponent<Rigidbody>();
        jh = new Vector3(0,thrust, 0);
        gravVect = new Vector3(0, gravityValue, 0);
    }

    void FixedUpdate() {
        if (Input.GetButton("Jump")) {
            rb.AddForce(jh, ForceMode.Force);
        }
       rb.velocity = new Vector3(rb.velocity.x, Mathf.Clamp(rb.velocity.y, Mathf.NegativeInfinity, clampVal), rb.velocity.z);
             rb.AddForce(gravVect, ForceMode.Force);
    }
}

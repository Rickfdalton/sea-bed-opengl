#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include <image_loader/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

Camera cam = Camera(glm::vec3(0.0f, 0.0f, 2.5f));
Camera_Movement cam_mov;
float dt, lastX, lastY, pitch, yaw, fov;
bool firstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    cam.process_mouse_move(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    cam.process_mouse_scroll(yoffset);
}


void process_input(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cam.process_keyboard(FORWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cam.process_keyboard(BACKWARD, dt);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cam.process_keyboard(LEFT, dt);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cam.process_keyboard(RIGHT, dt);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        cam.process_keyboard(UP, dt);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        cam.process_keyboard(DOWN, dt);
}



int main()
{
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    GLFWwindow* window = glfwCreateWindow(800, 400, "Hello Window", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize glad" << std::endl;
        return -1;
    }
    glViewport(0, 0, 800, 600);
    lastX = 400;
    lastY = 300;
    pitch = 0.0f;
    yaw = 0.0f;
    fov = 45.0f;

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //texture
    //stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load("C:/Users/Pc/Pictures/Capture.PNG", &width, &height, &nrChannels, 0);


    unsigned int texture1;
    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        cout << "TEXTURE LOAD FAILED" << endl;
    }
    stbi_image_free(data);



    float vertices[605];
    unsigned int indices[600];


    int idx = 0;
    for (int z = 0;z <= 10;z++) {
        for (int x = 0;x <= 10;x++) {
            vertices[idx] = x-5;idx++;
            vertices[idx] = 0;idx++;
            vertices[idx] = z-5;idx++;
            vertices[idx] = x;idx++;
            vertices[idx] = z;idx++;
        }
    }

    int i_1 = 1; 

    idx = 0;
    /*
    while (i_1 <= 109) {
        indices[idx] = i_1;idx++;
        indices[idx] = i_1-1;idx++;
        indices[idx] = i_1+11;idx++;
        indices[idx] = i_1+11;idx++;
        indices[idx] = i_1 + 12;idx++;
        indices[idx] = i_1 ;idx++;
        i_1++;
    }*/
    for (int z = 0; z < 10; z++) {
        for (int x = 0; x < 10; x++) {
            int topLeft = z * 11 + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * 11 + x;
            int bottomRight = bottomLeft + 1;

            // first triangle
            indices[idx++] = topLeft;
            indices[idx++] = bottomLeft;
            indices[idx++] = topRight;

            // second triangle
            indices[idx++] = topRight;
            indices[idx++] = bottomLeft;
            indices[idx++] = bottomRight;
        }
    }
    
    Shader ourShader("C:/Users/Pc/source/repos/rendering-sea/shader.vs", "C:/Users/Pc/source/repos/rendering-sea/shader.fs");


    unsigned int VBO; 
    glGenBuffers(1, &VBO);

    //bind VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    //configure VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //set texture attribute pointers 
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);


    ourShader.use();
    ourShader.setInt("texture1", 0);


    glm::mat4 projection;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    dt = 0.0f;
    float last_time = 0.0f;

    //render loop
    while (!glfwWindowShouldClose(window)) {

        float current_time = glfwGetTime();
        dt = current_time - last_time;
        last_time = current_time;

        process_input(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(VAO);

        ourShader.use();
        glm::mat4 model;
        glm::mat4 view;

        const float radius = 8.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;
        view = cam.get_view_mat();

        int viewLoc = glGetUniformLocation(ourShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        projection = glm::perspective(glm::radians(cam.zoom), 800.0f / 600.0f, 0.1f, 100.0f);


        int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        
        model = glm::mat4(1.0f);
        int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawElements(GL_TRIANGLES, 600, GL_UNSIGNED_INT, 0);

        

        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwTerminate();
    return 0;

}
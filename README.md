# ESC-from-Planet-X

## Overview
**ESC-from-Planet-X** is a fast-paced action game where players control a fugitive cyborg trying to escape from a heavily monitored planet. The goal is to avoid detection by security cameras while navigating dynamic terrain filled with challenging obstacles.

---

## Gameplay Mechanics

### 1. Dynamic Tiles  
The terrain constantly shifts, with tiles changing positions every second. If the player steps on an active tile, they are teleported back to the starting point, adding urgency and requiring precision in movement.

### 2. Reset Mechanic  
If the player collides with an active tile, they are reset to the starting position. This mechanic ensures that players must carefully plan their movements to progress.

---

## Implementation Details  

### Dynamic Tile Logic  
The dynamic tile mechanic is implemented using the **time module** to update tile positions every second. Below is the code snippet responsible for switching the tiles:

```cpp
if (deltaTime > 1.0f) {
    lastTime = glfwGetTime();
    glBindVertexArray(vao2);
    glm::mat4 model;
    model = glm::translate(model, positions[k]);
    glm::vec4 colorRect = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    unsigned int transformLocation = glGetUniformLocation(programID, "transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));
    unsigned int transformColor = glGetUniformLocation(programID, "color");
    glUniform4fv(transformColor, 1, glm::value_ptr(colorRect));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    k++;
    if (k == 3) k = 0;
} else {
    glBindVertexArray(vao2);
    glm::mat4 model;
    model = glm::translate(model, positions[k]);
    glm::vec4 colorRect = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
    unsigned int transformLocation = glGetUniformLocation(programID, "transform");
    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(model));
    unsigned int transformColor = glGetUniformLocation(programID, "color");
    glUniform4fv(transformColor, 1, glm::value_ptr(colorRect));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
}
```

### Reset Mechanic  
The reset mechanic ensures that players are sent back to the starting position if they collide with an active tile. Below is the logic responsible for handling the reset:

```cpp
if (k == 0 && currentX >= 0.06f && currentX <= 0.3f) {
    trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    currentX = LEFT_LIMIT;
} else if (k == 1 && currentX >= 0.3f && currentX <= 0.6f) {
    trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    currentX = LEFT_LIMIT;
} else if (k == 2 && currentX >= 0.6f && currentX <= 0.9f) {
    trans = glm::translate(glm::mat4(1.0f), glm::vec3(LEFT_LIMIT, -0.35f, 0.0f));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    currentX = LEFT_LIMIT;
}
```

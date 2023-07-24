from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
from pyquaternion import Quaternion
import matplotlib.pyplot as plt

# Define quaternion visualization parameters
QUAT_SCALE = 1.0
QUAT_LENGTH = 0.5

# Define quaternion (change the values to see different orientations)
quaternion = Quaternion(1.0, 0.0, 0.0, 0.0)  # (w, x, y, z)

def draw_axes():
    # Draw white axes
    glColor3f(1, 1, 1)
    glBegin(GL_LINES)
    glVertex3f(0, 0, 0)
    glVertex3f(1, 0, 0)
    glVertex3f(0, 0, 0)
    glVertex3f(0, 1, 0)
    glVertex3f(0, 0, 0)
    glVertex3f(0, 0, 1)
    glEnd()

    # Draw axis labels
    plt.rcParams['axes.linewidth'] = 2
    plt.text(1.2, 0, "X", fontsize=12, fontweight='bold')
    plt.text(0, 1.2, "Y", fontsize=12, fontweight='bold')
    plt.text(0, 0, "Z", fontsize=12, fontweight='bold')

def draw_quaternion():
    glPushMatrix()
    glScalef(QUAT_SCALE, QUAT_SCALE, QUAT_SCALE)

    # Convert quaternion to rotation matrix
    rotation_matrix = quaternion.rotation_matrix

    # Draw the quaternion visualization (red arrow)
    glBegin(GL_LINES)
    glColor3f(1, 0, 0)  # Red color
    glVertex3f(0, 0, 0)
    glVertex3f(QUAT_LENGTH * rotation_matrix[0][0], QUAT_LENGTH * rotation_matrix[1][0], QUAT_LENGTH * rotation_matrix[2][0])
    glEnd()

    # Draw the bold 'Q' symbol
    plt.text(QUAT_LENGTH * rotation_matrix[0][0] + 0.05, QUAT_LENGTH * rotation_matrix[1][0] + 0.05, "Q", fontsize=12, fontweight='bold', color='red')

    glPopMatrix()

def draw():
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    draw_axes()
    draw_quaternion()
    glutSwapBuffers()

def main():
    glutInit()
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH)
    glutInitWindowSize(800, 600)
    glutCreateWindow(b"Quaternion Visualization")
    glutDisplayFunc(draw)
    glutIdleFunc(draw)
    glMatrixMode(GL_PROJECTION)
    gluPerspective(45, (800 / 600), 0.1, 50.0)
    glMatrixMode(GL_MODELVIEW)
    glTranslatef(0.0, 0.0, -5)
    glutMainLoop()

if __name__ == "__main__":
    main()

#version 330 core

in float inValue;
out float geoValue;

void main(void)
{
    geoValue = sqrt(inValue);
}

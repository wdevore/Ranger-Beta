#version 450 core

out vec4 FragColor;

uniform vec4 textColor;

void main()
{
    // 1. Vector from center (0.5, 0.5) to current fragment (gl_PointCoord)
    vec2 pointVector = gl_PointCoord - vec2(0.5);

    // 2. Calculate squared distance from center.
    //    We use length() or dot(pointVector, pointVector) for efficiency.
    float distanceSquared = dot(pointVector, pointVector);

    // 3. If the distance is greater than the radius squared (0.5 * 0.5 = 0.25), discard.
    if(distanceSquared > 0.25) {
        discard;
    }

    // Optional: Add a smooth edge (anti-aliasing)
    float radius = 0.5;
    float alpha = 1.0 - smoothstep(radius * 0.9, radius, distanceSquared);
    FragColor = vec4(textColor.rgb, alpha);
    
    FragColor = vec4(textColor.rgb, 1.0);
}
#version 450 core

out vec4 FragColor;

uniform vec4 textColor;

void main()
{
    // 1. Calculate squared distance from the point's center (0.5, 0.5)
    vec2 pointVector = gl_PointCoord - vec2(0.5);
    float distSquared = dot(pointVector, pointVector);
    
    // Define the full radius squared
    float radiusSquared = 0.25; // 0.5 * 0.5
    
    // 2. Define the smooth step edges for the anti-aliasing band
    //    The inner edge (0.95 * radiusSquared) is where the fade starts.
    //    The outer edge (radiusSquared) is where the point is fully transparent.
    float innerEdge = radiusSquared - 0.05;  // 0.95 * radiusSquared;
    float outerEdge = radiusSquared;

    // 3. Use smoothstep to calculate the anti-aliasing alpha
    //    smoothstep(a, b, x) returns 0.0 when x <= a and 1.0 when x >= b,
    //    and smoothly interpolates in between.
    
    // We want alpha to be 1.0 inside the point, and 0.0 outside.
    // By inverting the subtraction, we achieve the fade from 1.0 to 0.0.
    float alpha = 1.0 - smoothstep(innerEdge, outerEdge, distSquared);

    // 4. Apply the color and the calculated alpha
    if (alpha < 0.01) { // Optional: Discard fully transparent fragments for performance
        discard;
    }

    FragColor = vec4(textColor.rgb, alpha);}
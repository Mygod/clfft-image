__kernel void inputX(__global char3 *bitmap, __global float2 *in, int width) {
  int x = get_local_id(0), y = get_local_id(1), i = y * width + x;
  in[i] = (float2) ((x + y) & 1 ? -(float) bitmap[i].x : bitmap[i].x, 0);
}
__kernel void inputY(__global char3 *bitmap, __global float2 *in, int width) {
  int x = get_local_id(0), y = get_local_id(1), i = y * width + x;
  in[i] = (float2) ((x + y) & 1 ? -(float) bitmap[i].y : bitmap[i].y, 0);
}
__kernel void inputZ(__global char3 *bitmap, __global float2 *in, int width) {
  int x = get_local_id(0), y = get_local_id(1), i = y * width + x;
  in[i] = (float2) ((x + y) & 1 ? -(float) bitmap[i].z : bitmap[i].z, 0);
}

__kernel void outputX(__global char3 *bitmap, __global float2 *out, int width) {
  int x = get_local_id(0), y = get_local_id(1), i = y * width + x;
  bitmap[i].x = sqrt(out[i].x * out[i].x + out[i].y * out[i].y);
}
__kernel void outputY(__global char3 *bitmap, __global float2 *out, int width) {
  int x = get_local_id(0), y = get_local_id(1), i = y * width + x;
  bitmap[i].y = sqrt(out[i].x * out[i].x + out[i].y * out[i].y);
}
__kernel void outputZ(__global char3 *bitmap, __global float2 *out, int width) {
  int x = get_local_id(0), y = get_local_id(1), i = y * width + x;
  bitmap[i].z = sqrt(out[i].x * out[i].x + out[i].y * out[i].y);
}

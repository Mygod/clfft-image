__kernel void inputX(__global char *bitmap, __global float2 *in, int width) {
  int x = get_global_id(0), y = get_global_id(1), i = y * width + x;
  in[i] = (float2) ((x + y) & 1 ? -(float) bitmap[i * 3] : bitmap[i * 3], 0);
}
__kernel void inputY(__global char *bitmap, __global float2 *in, int width) {
  int x = get_global_id(0), y = get_global_id(1), i = y * width + x;
  in[i] = (float2) ((x + y) & 1 ? -(float) bitmap[i * 3 + 1] : bitmap[i * 3 + 1], 0);
}
__kernel void inputZ(__global char *bitmap, __global float2 *in, int width) {
  int x = get_global_id(0), y = get_global_id(1), i = y * width + x;
  in[i] = (float2) ((x + y) & 1 ? -(float) bitmap[i * 3 + 2] : bitmap[i * 3 + 2], 0);
}

__kernel void outputX(__global char *bitmap, __global float2 *out, int width) {
  int x = get_global_id(0), y = get_global_id(1), i = y * width + x;
  bitmap[i * 3] = (char) 105.886458025 * log(1 + sqrt(out[i].x * out[i].x + out[i].y * out[i].y));
}
__kernel void outputY(__global char *bitmap, __global float2 *out, int width) {
  int x = get_global_id(0), y = get_global_id(1), i = y * width + x;
  bitmap[i * 3 + 1] = (char) 105.886458025 * log(1 + sqrt(out[i].x * out[i].x + out[i].y * out[i].y));
}
__kernel void outputZ(__global char *bitmap, __global float2 *out, int width) {
  int x = get_global_id(0), y = get_global_id(1), i = y * width + x;
  bitmap[i * 3 + 2] = (char) 105.886458025 * log(1 + sqrt(out[i].x * out[i].x + out[i].y * out[i].y));
}

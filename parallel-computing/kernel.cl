__kernel void hello_world(__global char16 *str) {
  *str = (char16) ('H', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '\0', '\0', '\0');
}

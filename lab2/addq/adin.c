#include <stdio.h>
int add(int a, int b) {
    return a + b;
}
void print_message(const char *msg) {
    printf("%s\n", msg);
}
void no_return_type_function() {
    printf("This function does not return anything.\n");
}

int* get_pointer_to_value(int value) {
    static int val;
    val = value;
    return &val;
}

void process_data(int a, int b, float c, double d) {
    printf("Processing data: %d, %d, %f, %f\n", a, b, c, d);
}

int main() {
    int sum = add(5, 10);
    printf("Sum: %d\n", sum);
    print_message("Hello, World!");
    no_return_type_function();
    int* ptr = get_pointer_to_value(42);
    printf("Pointer to value: %d\n", *ptr);
    process_data(1, 2, 3.14, 2.718);
    
    return 0;
}

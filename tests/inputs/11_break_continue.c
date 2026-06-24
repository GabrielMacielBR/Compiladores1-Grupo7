int main() {
    int i = 0;
    while (i < 10) {
        i++;
        if (i == 3) {
            continue;
        }
        if (i == 7) {
            break;
        }
    }
    return 0;
}
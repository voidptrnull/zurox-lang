# Changes and Features of Zurox

Here are some of the things that were changed/removed.

- Pointers and dereferncing are done by using keywords `ref` and `deref` respectively. This intends to get rid of confusing pointer arithmetic like this:
    ```c
    char*(*(*x[][8]));
    ```

    which in turn would look like 
    ```zx
    char ref (deref (deref x[][8])) 
    ```
    > Someone confirm this please

- Removal of while, for and do-while loops. While they could be benificial, since Zurox aims to be minimal, they were removed from Zurox. This is some sample code from C and Zurox, comparing them.
    ```c
    for (int i = 0;i < 10;i++) {
        // ... body
    }

    int i = 0;
    while (i < 10) {
        // ... body
        i++;
    }
    
    i = 1; // Reset
    do {
        // ... body
        ++i;
    } while (i < 10);

    ```

    ```zx
    i32 i = 0;
    loop {
        // ... body
        if (i == 10) {
            break;
        }
        i += 1;
    }
    ```
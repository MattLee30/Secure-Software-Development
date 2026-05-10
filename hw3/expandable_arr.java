import java.util.EmptyStackException;

class ExpandableArr {
    private static final int MIN_CAPACITY = 8;
    private static final int GROW_FACTOR = 2;

    private String[] items;
    private int size;

    ExpandableArr() {
        items = new String[MIN_CAPACITY];
        size = 0;
    }

    void push(String str) {
        if (str == null) {
            throw new IllegalArgumentException("cannot push null string");
        }
        if (size == items.length) {
            resize(items.length * GROW_FACTOR);
        }
        items[size++] = str;
    }

    String pop() {
        if (size == 0) {
            throw new EmptyStackException();
        }
        String val = items[size - 1];
        items[--size] = null;
        if (items.length > MIN_CAPACITY && size <= items.length / 4) {
            resize(items.length / GROW_FACTOR);
        }
        return val;
    }

    String peek() {
        if (size == 0) {
            throw new EmptyStackException();
        }
        return items[size - 1];
    }

    int size() {
        return size;
    }

    boolean isEmpty() {
        return size == 0;
    }

    private void resize(int newCapacity) {
        if (newCapacity < MIN_CAPACITY) {
            newCapacity = MIN_CAPACITY;
        }
        String[] newItems = new String[newCapacity];
        System.arraycopy(items, 0, newItems, 0, size);
        items = newItems;
    }

    public static void main(String[] args) {
        ExpandableArr s = new ExpandableArr();

        try {
            s.pop();
            System.out.println("FAIL: expected EmptyStackException on pop");
        } catch (EmptyStackException e) {
            System.out.println("PASS: pop from empty throws");
        }

        try {
            s.peek();
            System.out.println("FAIL: expected EmptyStackException on peek");
        } catch (EmptyStackException e) {
            System.out.println("PASS: peek from empty throws");
        }

        try {
            s.push(null);
            System.out.println("FAIL: expected IllegalArgumentException on null push");
        } catch (IllegalArgumentException e) {
            System.out.println("PASS: push null throws");
        }

        s.push("hello");
        s.push("world");
        System.out.println((s.peek().equals("world") ? "PASS" : "FAIL") + ": peek returns top");
        System.out.println((s.size() == 2 ? "PASS" : "FAIL") + ": size is 2");
        System.out.println((!s.isEmpty() ? "PASS" : "FAIL") + ": stack is not empty");
        System.out.println((s.pop().equals("world") ? "PASS" : "FAIL") + ": pop returns 'world'");
        System.out.println((s.pop().equals("hello") ? "PASS" : "FAIL") + ": pop returns 'hello' (LIFO)");
        System.out.println((s.isEmpty() ? "PASS" : "FAIL") + ": stack is empty after all pops");

        for (int i = 0; i < 20; i++) {
            s.push("item" + i);
        }
        System.out.println((s.size() == 20 ? "PASS" : "FAIL") + ": size is 20 after expansion");
        System.out.println((s.pop().equals("item19") ? "PASS" : "FAIL") + ": pop after expansion returns correct item");
    }
}

class Warehouse {
    private int count = 100; // 初始化仓库的货物数量为100
    private final int capacity = 100; // 仓库容量为100

    public synchronized void get(int num) {
        while (count < num) { // 使用while循环处理假唤醒
            try {
                System.out.println("暂不能取货，货物不够！");
                wait(); // 等待被唤醒
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        count -= num;
        System.out.println("可以取货，取了" + num + "件，还剩" + count + "件。");
        notifyAll(); // 唤醒所有等待的线程
    }

    public synchronized void put(int num) {
        while (count + num > capacity) { // 使用while循环处理假唤醒
            try {
                System.out.println("暂不能存货，容量有限！");
                wait(); // 等待被唤醒
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        count += num;
        System.out.println("可以存货，存了" + num + "件，还剩" + count + "件。");
        notifyAll(); // 唤醒所有等待的线程
    }
}

class Worker implements Runnable {
    private Warehouse warehouse;
    private boolean isStoring; // 标记此工人是存货还是取货

    public Worker(Warehouse warehouse, boolean isStoring) {
        this.warehouse = warehouse;
        this.isStoring = isStoring;
    }

    @Override
    public void run() {
        for (int i = 0; i < 5; i++) { // 每个线程执行5次操作
            if (isStoring) {
                warehouse.put(40); // 存货
            } else {
                warehouse.get(60); // 取货
            }
        }
    }
}

public class Main {
    public static void main(String[] args) {
        Warehouse warehouse = new Warehouse();
        Worker worker1 = new Worker(warehouse, false); // 创建一个取货的工人
        Worker worker2 = new Worker(warehouse, true); // 创建一个存货的工人

        new Thread(worker1).start(); // 启动取货的工人线程
        new Thread(worker2).start(); // 启动存货的工人线程
    }
}

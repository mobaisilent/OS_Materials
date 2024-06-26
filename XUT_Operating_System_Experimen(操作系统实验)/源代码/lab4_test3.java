public class ThreadDemo2 {
    public static void main(String[] args){
        Demo2 d =new Demo2();
        Thread t = new Thread(d);
        t.start();
        for(int x=0;x<60;x++){
            System.out.println(Thread.currentThread().getName()+x);
        }
    }
}

class Demo2 implements Runnable{
    public void run(){
        for(int x=0;x<60;x++){
            System.out.println(Thread.currentThread().getName()+x);
        }
    }
}
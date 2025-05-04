package threads;

public class SimpleThread {

    private static void case1() throws InterruptedException {
        Thread t1 = new Thread(() -> {            
            try {
                Thread.sleep(1000);                
            } catch (InterruptedException e) {                
            }
            System.out.println("message from thread");            
        });
        t1.start();
        
        //thread blocks
        //Thread.currentThread().join();

        System.out.println("exiting main thread");

        //Unlike pthreads, the main thread ending its execution does not stop the other threads
        //System.exit(0);        
    }

    /*
     * create big amount of threads without joining
     */
    private static void case2() throws InterruptedException {

        int numThreads = 100_000;
        for(int j=0; j<numThreads; j++) {
            if(j%(numThreads/10) == 0) {
                System.out.println("new thread created - " + j); 
            }
            new Thread(() -> {            
                //System.out.println("message from thread " + Thread.currentThread().getId());            
            }).start();    
        }    
        
        System.out.println("exiting main thread");       
    }

    /*
     * create big amount of threads with joining
     */
    private static void case3() throws InterruptedException {

        int numThreads = 1_000_000;
        for(int j=0; j<numThreads; j++) {
            if(j%(numThreads/10) == 0) {
                System.out.println("new thread created - " + j); 
            }
            Thread t = new Thread(() -> {            
                //System.out.println("message from thread " + Thread.currentThread().getId());            
            });
            t.start();
            t.join();

        }    
        
        System.out.println("exiting main thread");       
    }

    /*
     * create as many threads as possible
     */
    private static void case4() throws InterruptedException {

        int numThreads = 10_000;
        for(int j=0; j<numThreads; j++) {
            if(j%(numThreads/10) == 0) {
                System.out.println("new thread created - " + j); 
            }
            Thread t = new Thread(() -> {   
                try {
                    Thread.sleep(30000);
                } catch (InterruptedException e) {
                }
                         
                //System.out.println("message from thread " + Thread.currentThread().getId());            
            });
            t.start();            

        }    
        
        System.out.println("exiting main thread");       
    }

    public static void main(String[] args) throws InterruptedException {        
        case4();
    }
}
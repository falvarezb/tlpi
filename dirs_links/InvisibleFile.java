
import java.io.*;

public class InvisibleFile {
    public static void main(String args[]) throws InterruptedException, IOException {
        File invisibleFile = new File("invisiblefile");
        invisibleFile.createNewFile();
        FileWriter fw = new FileWriter(invisibleFile);
        System.out.println("file created");
        Thread.sleep(5000);

        boolean deleted = invisibleFile.delete();
        if(!deleted) {
            System.out.println("error deleting file");
            System.exit(1);
        }
        fw.write("hello");       
        fw.flush();         
        System.out.println("file deleted");
        // 'ls invisiblefile' does not return anything
        // but the file is still held open by the process:
        // lsof -p $(ps -ef | awk '/[I]nvisibleFile.java/ {print $2}') | awk '/invisiblefile/ {print "size:" $7; print "inode:" $8}'
        Thread.sleep(5000);

        fw.close();
        System.out.println("file closed");
        Thread.sleep(5000);
        // after closing file, it is completely gone
        
        System.out.println("end");
    }
}
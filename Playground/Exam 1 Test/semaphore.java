public class semaphore {
	private static int in_use = 0;
	public synchronized void sem_wait(){
		while (in_use <= 0)
			wait();

		in_use --;
	}

	public synchronized void sem_post{
		in_use ++;
		notify();
	}
}
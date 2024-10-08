<h1>Dining Philosophers Problem - Embedded Systems Solution</h1>

<h2>Example Output Analysis (with Logic Analyzer)</h2>

<pre>
P1 Think [__/‾‾]
P1 Fork  [_/‾‾‾]
P1 Eat   [/‾‾‾‾]
</pre>

<h2>System Requirements</h2>

<ul>
  <li>MCU with RTOS support (FreeRTOS recommended)</li>
  <li>Minimum 32KB RAM</li>
  <li>Hardware mutex support preferred</li>
</ul>

<h2>Critical Considerations for Embedded Systems</h2>

<p>Each philosopher requires:</p>
<ul>
  <li>Task handle: 48 bytes</li>
  <li>Mutex handle: 8 bytes</li>
  <li>Status variables: 4 bytes</li>
  <li>Total per philosopher: ~60 bytes</li>
</ul>

<h2>The Dining Philosophers Challenge</h2>

<h3>What's The Big Deal?</h3>
<p>Imagine this: You’ve got a bunch of philosophers sitting around a table, each with a fork between them. They need two forks to eat but can only grab the ones next to them. This is a classic computer science problem that teaches about resource sharing and deadlock prevention.</p>

<h3>Core Concepts You Need to Know</h3>

<h4>1. Threading and Race Conditions</h4>
<p>Think of threads like separate cooks in a kitchen. When they all try to use the same knife (resource), chaos ensues. This is a race condition – multiple threads trying to access shared data. Example:</p>

<pre>
<code>
int shared_counter = 0;

void *increment_counter(void *arg) {
    for (int i = 0; i &lt; 1000000; i++) {
        shared_counter++;  // Danger zone!
    }
    return NULL;
}
</code>
</pre>

<h4>2. Mutexes: Your New Best Friend</h4>
<p>Mutexes are like locks on a bathroom door – one person at a time! They prevent race conditions:</p>

<pre>
<code>
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int shared_counter = 0;

void *safe_increment(void *arg) {
    for (int i = 0; i &lt; 1000000; i++) {
        pthread_mutex_lock(&mutex);
        shared_counter++;  // Safe zone!
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
</code>
</pre>

<h2>Building Your Solution</h2>

<h3>1. Data Structures</h3>

<p>You’ll need two main structures:</p>

<pre>
<code>
typedef struct s_philosopher {
    int id;
    pthread_t thread;
    struct s_data *shared_data;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    uint64_t last_meal_time;
    int meals_eaten;
} t_philosopher;

typedef struct s_data {
    t_philosopher *philosophers;
    pthread_mutex_t *forks;
    pthread_mutex_t write_lock;
    int philosopher_count;
    uint64_t time_to_die;
    uint64_t time_to_eat;
    uint64_t time_to_sleep;
    int required_meals;
    int simulation_stop;
} t_data;
</code>
</pre>

<h3>2. The Philosopher's Life Cycle</h3>

<p>Each philosopher follows this routine:</p>
<ul>
  <li>Think</li>
  <li>Get hungry</li>
  <li>Pick up forks</li>
  <li>Eat</li>
  <li>Put down forks</li>
  <li>Sleep</li>
  <li>Repeat</li>
</ul>

<p>Here’s a basic implementation:</p>

<pre>
<code>
void *philosopher_routine(void *arg) {
    t_philosopher *philo = (t_philosopher *)arg;
    
    while (!simulation_should_stop(philo->shared_data)) {
        think(philo);
        if (get_forks(philo)) {
            eat(philo);
            put_forks(philo);
            sleep_philosopher(philo);
        }
    }
    return NULL;
}
</code>
</pre>

<h3>3. Vital Functions</h3>

<h4>Time Management</h4>

<pre>
<code>
uint64_t get_current_time(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void precise_sleep(uint64_t milliseconds) {
    uint64_t start = get_current_time();
    while (get_current_time() - start &lt; milliseconds) {
        usleep(100);  // Short sleep to prevent CPU hogging
    }
}
</code>
</pre>

<h4>Death Checker</h4>

<pre>
<code>
void *death_checker(void *arg) {
    t_data *data = (t_data *)arg;
    
    while (!data->simulation_stop) {
        for (int i = 0; i &lt; data->philosopher_count; i++) {
            if (time_since_last_meal(&data->philosophers[i]) > data->time_to_die) {
                announce_death(&data->philosophers[i]);
                data->simulation_stop = 1;
                return NULL;
            }
        }
        usleep(1000);
    }
    return NULL;
}
</code>
</pre>

<h2>Common Pitfalls and How to Avoid Them</h2>

<ul>
  <li><strong>Deadlock:</strong> If every philosopher grabs their right fork and waits for the left, you’ve got deadlock. Solution: Make one philosopher grab forks in reverse order, or use a semaphore.</li>
  <li><strong>Resource Starvation:</strong> Some philosophers might never get to eat. Solution: Implement a maximum wait time or use a fair locking mechanism.</li>
  <li><strong>Race Conditions:</strong> Always protect shared resources with mutexes, but be careful not to create deadlock while doing so!</li>
</ul>

<h2>Testing Your Solution</h2>

<h3>Basic Functionality</h3>

<pre>
<code>
./philo 5 800 200 200  # Should run indefinitely
./philo 5 800 200 200 7  # Should stop after each philosopher eats 7 times
</code>
</pre>

<h3>Edge Cases</h3>

<pre>
<code>
./philo 1 800 200 200  # Philosopher should die
./philo 4 310 200 100  # Should demonstrate a death
</code>
</pre>

<h3>Error Handling</h3>

<pre>
<code>
./philo 0 800 200 200  # Should error gracefully
./philo 5 -800 200 200  # Should error gracefully
</code>
</pre>

<h2>Memory Management Tips</h2>

<ul>
  <li>Always initialize mutexes before use</li>
  <li>Destroy mutexes before program exit</li>
  <li>Free all allocated memory</li>
  <li>Use Valgrind to check for leaks</li>
</ul>

<pre>
<code>
void cleanup(t_data *data) {
    for (int i = 0; i &lt; data->philosopher_count; i++) {
        pthread_mutex_destroy(&data->forks[i]);
    }
    pthread_mutex_destroy(&data->write_lock);
    free(data->philosophers);
    free(data->forks);
}
</code>
</pre>

<h2>Final Thoughts</h2>

<p>The dining philosophers problem isn’t just about virtual philosophers; it’s about understanding concurrent programming, resource sharing, and deadlock prevention. These concepts are crucial in real-world applications, from operating systems to database design.</p>

<h3>Remember:</h3>

<ul>
  <li>Always protect shared resources</li>
  <li>Be mindful of deadlock</li>
  <li>Test thoroughly, especially edge cases</li>
  <li>Clean up your resources</li>
</ul>

<p>Now go forth and let your philosophers dine in peace and harmony!</p>

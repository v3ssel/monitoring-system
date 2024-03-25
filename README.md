# MonitoringSystem
In this project, an application for monitoring systemic
OS indicators in the concept of core-agents. Agents are
dynamic library implementing the **Agent** interface, and the kernel is their
manager - it manages their loading, enabling/disabling, processes 
metrics, etc. This means that you can
create and load your own agent right at runtime. 
program.

## Configuration files
As an example, four different agents have been implemented. Each agent has its own configuration file, the default name of which is constructed from 
**.conf** + **Agent::type**, all configuration files must be in the same directory.

The configuration file is read at agent initialization, and thereafter at specified intervals, defaulting to 2 seconds.
The following parameters can be changed in the configuration file:

* Agent Name;

* Agent type;

* Metrics update time;

* Any additional settings in the **key**:**value** format.

* Next, you can add critical values for each metric
of a particular agent using a simple syntax:
**metric_name**(<,<=,>,>=,==,!=)**crit_value**

### All settings can be changed through the program interface.

## Notifications
Every time a metric reaches a critical value, you can get a 
notification to your **Telegram** and **Email** by clicking on the bottom
on the bottom 
right square and open notification settings, enable the necessary and
leave details of where to send messages:
* For **Telegram** this is your **UserId**, you can find it out via the official 
bot `@getmyid_bot`.
* For **Email** this is your mail address

### P.S. Notifications require `curl` installed and specified in your PATH to work

## Installation
Follow these steps to build and install the project:

1. **Clone the repository:**
    ```
    git clone [Repository URL]
    ```

2. **Go to the project directory:**
    ```
    cd src
    ```

3. **Install the project:**
    ```
    make install
    ```

The executable file and the **libMonitoringKernel** library needed for the project will be in `src`.

## Install agents
Follow these steps to build all agents:

1. **Go to the project directory:**
    ```
    cd src
    ```

2. **Installation:**
    ```
    make agents
    ```
All built agents will be placed in `./agents`.

In addition to this you can also build a specific agent, to do this use the
a specific target for the agent.

```
make AgentCPU
make AgentMemory
make AgentNetwork
make AgentExtra
```

## Usage
To start the kernel, press the large **START** button.

To stop the kernel, PCM click on the metrics window and select **STOP KERNEL**, 
it will take time for the kernel to come to a complete stop, after which you will return to the
start screen.

By default all agents are looked for in the `./agents` directory relative to the 
executable file, to change this click on the `./agents` directory on the window with the 
loaded agents
(top left square) and select **Change search directory.**;

Similarly, by default, configuration files are searched in the `./config` folder and the 
agent results are written to `./logs` relative to the executable, they
can also be changed.

You can also disable the search for new agents and recording of new metrics,
right-click on the desired box and select the necessary setting.

By selecting a particular agent from the list you can see its settings and time of
working time. All settings can be changed, the configuration file will be overwritten.

## Uninstall
To delete an installed project, follow these steps:

1. **Go to the project directory:**
    ```
    cd src
    ```

2. **Delete the project:** `` ``
    ```
    make uninstall
    ```
The uninstallation will follow the default paths, if any files have been moved they will not be deleted.

## Testing
Follow these steps to build and run tests for the project:

1. **Go to the project directory:**
    ```
    cd src
    ```

2. **Build and run tests:** **
    ```
    make tests
    ```

An executable will be created in `src`.

Testing takes time, it takes time to start and stop the kernel.

## Build project archive
Follow these steps to build a project archive:

1. **Go to the project directory:**
    ```
    cd src
    ```

2. **Build the archive:** `` ``
    ```
    make dist
    ```

## Cleanup of build files
To remove all temporary and assembly files created during compilation and testing, perform the following steps:

1. **Go to the project directory:**
    ```
    cd src
    ```

2. **Clearing the project:**
    ```
    make clean
    ```

## Create your own agent
To create your own agent:

1. Create a class that inherits the abstract class 
**Agent** - `./core/agents/Agent.h`.

2. Create a simple object creation function so the kernel can find your agent, replace **AgentCPU** with your class name:
    ```
    extern "C" AgentCPU* create_obj() {
        return new AgentCPU;
    }
    ```

3. Set the parameters **name**, **type**, **config_name**, **update_time_**  default, example:
    ```
    Agent::name = "AgentCPU";
    Agent::type = "CPU";
    Agent::config_name = ".conf" + Agent::type;
    Agent::update_time_ = 3;
    ```

4. Add the names of the metrics to be tracked:
    ```
    Agent::addMetric("cpu");
    Agent::addMetric("processes");
    ```

5. [Optional] You can also add a critical default value using:
    ```
    Agent::addCriticalComparison("cpu", Comparisons<double>::is_equal, CompareType::IS_EQ);
    Agent::addCriticalValue("cpu", std::numeric_limits<double>::max());
    ```

6. If your agent will read any configuration file it needs to create **config_reader_**, if you are happy with the format of the agents in the example, then you can use **AgentConfigReader** - `./core/agents/AgentConfigReader.h`. Otherwise, you can inherit the ConfigReader class and
set your own logic.
    ```
    Agent::config_reader_ = std::make_unique<AgentConfigReader>(this);
    ```

7. Implement the **updateMetrics()** method, in it you should implement the logic of
reading the data and sending the result to your observer, don't forget to check if
its presence. Use **observer_->NotifyError** to notify you when an error occurs.
errors, **observer_->NotifyCritical** for critical metrics, and
**observer_->NotifyResult** to send the result.

8. Implement the **toString()** method, in it you can nicely represent the last
result of metrics reading, for example in the format:
    ```
    <Metric1> : <Value1> | <Metric2> : <Value2> | <Metric3> : <Value3> | ....
    ```

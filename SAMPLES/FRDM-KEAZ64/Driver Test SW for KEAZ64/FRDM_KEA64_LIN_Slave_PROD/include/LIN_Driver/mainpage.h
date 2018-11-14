/** @mainpage LIN Driver Documentation
*
* @authors FPT Software<br>Copyright&nbsp;by&nbsp;Freescale&nbsp;Semiconductor,&nbsp;2011.
*
* <BR>
* <h1>Introduction</h1>
* <ul>
* <li><h2>System Architecture</h2></li>
* <P>
* The layered architecture of the <A href="group___l_i_n21__core__api__group.html">LIN2.1</A>/ <A href="group___j2602__core__api__group.html">J2602</A> Driver package is shown on <A HREF="#figure1">Figure 1</A>. Such architecture approach aims maximum reusability of 
* common code base for <A href="group___l_i_n21__core__api__group.html">LIN2.1</A> and <A href="group___j2602__core__api__group.html">J2602</A> standards for 8 bit and 16 bit Freescale automotive MCU portfolio. </P>
* <P>The core API layer of <A href="group___l_i_n21__core__api__group.html">LIN2.1</A>/ <A href="group___j2602__core__api__group.html">J2602</A> handles initialization, processing and signal based interaction between application and LIN Core. 
* The <A href="group___l_i_n21__core__api__group.html">LIN2.1</A> TL (Transport Layer) provides methods for tester to transmit diagnostic requests. </P>
* <P>The low level layer offers method of handling signal transmission between user application and hardware independence 
* such as byte sending, response receiving, break symbol detection, etc. </P>
* <P>The physical transport layer of the Driver supports three standard interfaces SCI, SLIC, GPIO to operate with 8 bit and 16 bit MCU hardware.</P>
* <A NAME="figure1"></A>
* @image html LIN_Driver_Architecture.jpg
* <CENTER><i>Figure 1.  LIN Driver Architecture diagram</i></CENTER>
* <hr>
* <li><h2>Supported Derivatives</h2></li>
*   <p>The following table displays the list of supported MCU derivatives including the functionalities. Note that all 
* derivatives support the LIN 2.0, <A href="group___l_i_n21__core__api__group.html">LIN2.1</A> and <A href="group___j2602__core__api__group.html">J2602</A> variant.
*
* @image html support.jpg
* <CENTER><i>Figure 2.  Supported Derivatives</i></CENTER>
*
* <hr>
* <li><h2>LIN Driver Package Components</h2></li>
*
* LIN Driver Package consists of two major parts:
*
* <ul>
*	<li>Node Configuration Tool: PC based script for LIN Driver configuration generation.</li>
*
*	<li>LIN Driver: Embedded SW package supporting the LIN 2.0, <A href="group___l_i_n21__core__api__group.html">LIN2.1</A> and <A href="group___j2602__core__api__group.html">J2602</A> communication</li>
* </li>
* </ul>
* </ul>
* <p>
* <p style="padding-left:20px">
* <ol>
* <li><B>Node Configuration Tool:</b></li><br>
* 
* The Node Configuration Tool is a built-in script of the LIN Driver package which allows user to easily generate the node 
* configuration .h and .c files based on LIN Configuration Description File (LCF) and Node Private Description File (LPF). 
* Those files are then in compiler integrated with LIN driver source code and user application and after compilation downloaded 
* to the target derivative. <A HREF="#figure2">Figure 3</A>. shows the diagram of configuration data flow.
* <A NAME="figure2"></A>
* @image html ConfigurationData.JPG
* <CENTER><i>Figure 3.  Configuration data</i></CENTER>
*
* The LDF file describes a complete LIN cluster including Master/slave mode definition and contains information to handle the cluster. <BR>
* The NPF file contains information about LIN nodes such as node name, number of interface, MCU clock frequency, used communication channel
*  (e.g. SCI channel) and port (e.g. GPIO port), etc., required for full description of the node.
*
* <li><B>LIN Driver:</B></li><br>
* 
* The <A href="#figure3">Figure 4</A>. shows the details of modules in the LIN driver package. It also demonstrates the relationship among modules and the direction of function call among them.
* <A NAME="figure3"></A>
* @image html LIN_Driver_Architecture1.jpg
* <CENTER><i>Figure 4.  LIN Driver Layer Diagram</i></CENTER>
*
* 
* LIN driver software package provides support for <A href="group___l_i_n21__core__api__group.html">LIN2.1</A>, LIN 2.0 and <A href="group___j2602__core__api__group.html">J2602</A> communication protocols. The driver package is divided into the layers as follows:
* 
* <ol type=I>
* <li>The lowest layer, <a href="group__bsp__group.html">Board Support Package</a> (BSP) layer is comprised of codes, which implements the tasks dedicated to specific MCU platform: 
* interrupt service routines, i/o port setup, memory handling and so on. There are three interfaces implemented within the driver package: <a href="group___s_c_i__group.html">SCI</a>, <a href="group___s_l_i_c__group.html">SLIC</a>, <a href="group___g_p_i_o__group.html">GPIO</a> and <a href="group___x_g_a_t_e__group.html">XGATE</a>.</li>
*
* <li><a href="group__lowlevel__group.html">Low level layer</a> consists of core functions for the LIN protocol such as the frames handling, signals transmission and reception, 
* data preparation, etc. Besides, this layer contains common implementation functions for the lowest layer to provide the interface abstraction. 
* Function for LIN cluster setup can be found here as well. This layer interacts with the core API layer through low level API functions.</li>
*
* <li><a href="group__coreapi__group.html">Core API layer</a> consists of API functions as defined by the <A href="group___l_i_n21__core__api__group.html">LIN2.1</A>/<A href="group___j2602__core__api__group.html">J2602</A> specification enabling the user to utilize the <A href="group___l_i_n21__core__api__group.html">LIN2.1</A>/<A href="group___j2602__core__api__group.html">J2602</A> 
* communication within the user application. Both the static and dynamic modes for calling the API functions are supported. The core API layer 
* interacts with the low level layer and can be called by such upper layers as <A href="group___l_i_n21__core__api__group.html">LIN2.1</A> TL API, LIN TL <A href="group___j2602__core__api__group.html">J2602</A> or application for diagnostic implementation</li>
*
* <li><a href="group__transport__group.html">Transport layer</a> stands between the application layer and the core API layer including <A href="group___l_i_n21__core__api__group.html">LIN2.1</A> TL API and LIN TL <A href="group___j2602__core__api__group.html">J2602</A>. 
* This layer provides APIs for the transport protocol, node configuration and diagnostic. For <A href="group___l_i_n21__core__api__group.html">LIN2.1</A>, all components will be 
* extended from LIN 2.0 specification. The node configuration for <A href="group___j2602__core__api__group.html">J2602</A> implements only some functions of LIN 2.0 specification. 
* The layer contains some main components below: 
* <ul>
* <li>Transport protocol:
*   <ul>
*    <li><A href="group___l_i_n21__core__api__group.html">LIN2.1</A>: Error handing and communication functions are added to LIN 2.0 specification.</li>
*   </ul>
* </li>
*
* <li>Node configuration:
*  <ul>
*    <li><A href="group___l_i_n21__core__api__group.html">LIN2.1</A> extends slave configuration and assign frame with ID range to LIN 2.0. The assign frame with ID is removed.</li>
*
*    <li><A href="group___j2602__core__api__group.html">J2602</A> simplifies LIN 2.0 Node configuration.</li>
*   </ul>
* </li>
*
* <li><a href="group__diagnostic__group.html">Diagnostic</a>:
*
*  <ul>
*   <li>LIN 2.0 Diagnostic implements functions as diagnostic service. In the other hand, <A href="group___l_i_n21__core__api__group.html">LIN2.1</A> Diagnostic implements functions.</li>
*  </ul>
* </li>
* </ul>
* </li>
*
* <li>Application layer is the highest layer which stands for applications of user.</li>
* </ol>
* </ol>
* 
* </p>
* 
*/

/**
* @defgroup LIN_Driver LIN driver */

/** @defgroup diagnostic_group Diagnostic services
* @ingroup LIN_Driver 
* @brief Diagnostic services defines methods to implement diagnostic data transfer between a master node, respectively a diagnostic tester, and the slave nodes
* @details Three different classes of diagnostic nodes are defined. Class I is using normal signalling and class II and class III uses the transport layer.<br><br>
* The master node and the diagnostic tester are connected via a back-bone bus (e.g. CAN). The master 
* node shall receive all diagnostic requests addressed to the slave nodes from the 
* back-bone bus, and gateway them to the correct LIN cluster(s). Responses from the 
* slave nodes shall be gatewayed back to the back-bone bus through the master node. <br><br>
* All diagnostic requests and responses (services) addressed to the slave nodes can be 
* routed in the network layer  (i.e. no application layer routing), if the Diagnostic and 
* Transport Layer Protocol of  tester back-bone-bus master node fulfills the respective 
* needs. In this case, the master node must implement the LIN transport protocol, see 
* Transport Layer Specification, as well as the transport protocols used on the back-
* bone busses (e.g. ISO15765-2 on CAN)
*/

/** @defgroup transport_group Transport layer API
* @ingroup LIN_Driver 
* @brief Transport layer stands between the application layer and the core API layer
* @details
* This layer consists the implementation of data transportation which contains one or more LIN frames.
* It is situated between the application layer and the core API layer including LIN2.1 TL API and LIN TL J2602. 
* This layer provides APIs for the transport protocol, node configuration and diagnostic. 
* For LIN 2.1, all components will be extended from LIN 2.0 specification. 
* The node configuration for J2602 implements only some functions of LIN 2.0 specification
*/

/** @defgroup coreapi_group LIN Core API
* @ingroup LIN_Driver 
* @brief Refer to chappter 7, LIN 2.1 specification
* @details 
*   - Core API layer consists of API functions as defined by the LIN2.1/J2602 specification
*   - Enabling the user to utilize the LIN2.1/J2602 communication within the user application. 
*   - Both the static and dynamic modes for calling the API functions are supported. 
*   - The core API layer interacts with the low level layer and can be called by such upper layers as LIN2.1 TL API, 
* LIN TL J2602 or application for diagnostic implementation
*/

/** @defgroup lowlevel_group Low level API
* @ingroup LIN_Driver 
* @brief  Low level layer consists of core functions for the LIN protocol
* @details This layer contains the implementation of LIN protocol such as the frames handling, 
* signals transmission and reception, data preparation, etc. Besides, this layer contains common implementation functions 
* for the lowest layer to provide the interface abstraction. Function for LIN cluster setup can be found here as well. 
* This layer interacts with the core API layer through low level API functions.
*/

/** @defgroup bsp_group Board Support Package
* @ingroup LIN_Driver 
* @brief BSP  (Board Support Package) is the lowest layer of the LIN Driver.
* @details BSP is comprised of codes, 
* which implements the tasks dedicated to specific MCU platform: interrupt service routines, i/o port setup, memory handling and so on. 
* There are four interfaces implemented within the driver package: SCI, SLIC, GPIO and SCI + XGATE.
*/

/** @defgroup common_core_api_group Common Core API
* This group contains general core APIs that used for both protocol LIN 2.1 and J2602.
*
* @ingroup coreapi_group */
/** @defgroup LIN21_core_api_group LIN 2.1 Specific API
* LIN 2.1 is extended from in LIN 2.0 specification through diagnostic services and few functions were removed as obsolete.
* <ol>
* <li>LIN 2.1 is compatible with LIN 2.0:
*
* <ul>
* <li>A LIN 2.1 master node may handle a LIN 2.0 slave node if the master node also contains 
* all functionality of a LIN 2.0 master node, e.g. obsolete functions like Assign frame Id. </li>
*
* <li>A LIN 2.1 slave node can be used in a cluster with a LIN 2.0 master node if the LIN 2.1 slave node 
* is pre-configured, i.e. the LIN 2.1 slave node has a valid configuration after reset.</li>
* </ul>
* </li>
*
* <li>Changes between LIN 2.0 and LIN 2.1
*
* LIN2.1 enhance the capacity of LIN2.0 on event-triggered frame collision handling and diagnostic services supported.
* Besides, several features are add to fulfill powerful capacity of LIN network such as configuration service, assign frame ID range configuration, etc.
* </li></ol>
* @ingroup coreapi_group */
/** @defgroup J2602_core_api_group J2602 Specific API
* J2602 protocol is LIN 2.0 based. It contains LIN 2.0's modules to support Signal management, network management, scheduler and J2602 status management.
* The goal of J2602 is to improve the interoperability and interchangeability of LIN devices within a network by resolving those LIN2.0 requirements that are ambiguous, conflicting, or optional.
* Moreover, J2602 provides additional requirements that are not present in LIN2.0. For example: fault tolerant, operation, network topology, etc.
* Different to LIN2.1 protocol, J2602 does not support sporadic and event trigger frames in communication. 
* @ingroup coreapi_group */

/** @defgroup GPIO_group GPIO interface
* GPIO interface uses GPIO pins to communicates with LIN network. This group provides functions that help higher layers communicate with it. Only 9S12QD4 is supported.
*
* @ingroup bsp_group */
/** @defgroup SLIC_group SLIC interface
* SLIC (Slave LIN Interface Controller) is one integrated module of MCU, it support MCU communicating with LIN network as slave node. Only 9S08EL32 is supported.
*
* @ingroup bsp_group */
/** @defgroup SCI_group SCI interface
* This group contains functions that help physical module can communicate with LIN network through SCI module.
* List of supported MCUs with version of SCI and number of SCI channels is in table below:
* @image html SCI_allversions.png
*
* @ingroup bsp_group */
/** @defgroup XGATE_group XGATE interface
* This interface use SCI to communicate with LIN network and XGATE to process MCU's interrupts.
*
* @ingroup bsp_group */
/** @defgroup resync_group Resynchronize module
* Local interconnect network (LIN) is widely used standard for low cost automotive networks. 
* In order to ensure reliable communication via LIN bus, a bus clock of MCU needs to be accurate enough to avoid errors. 
* MCU can use crystal or ceramic resonator to provide very accurate bus clocks. However, LIN protocol was designed to allow more cost-effective solution. 
* An automatic resynchronization feature allows more cost-effective solution:  MCUs can use on-chip oscillators to implement LIN slaves, even though the on-chip oscillators have less accuracy than a crystal. 
* The automatic resynchronization module use the LIN SYNC field which is composed of five falling edges to synchronize with the master baud rate. 
* The module measures the sync byte field and adjusts the slave bus frequency after each LIN sync field reception from the master node.
*
* @ingroup bsp_group */

/** @defgroup 9s08qd4_group 9S08QD4 MCU
* @ingroup GPIO_group */
/** @defgroup 9s08el32_group 9S08EL32 MCU
* @ingroup SLIC_group */

/** @defgroup driver_cluster_group Driver and cluster management
*
* @brief initialization for LIN node.
*
* @details
*
* @ingroup common_core_api_group */
/** @defgroup signal_interract_group Signal interaction
*
* @brief This group contains APIs that help you communicate with signals of LIN node.
*
* @details
*
* @ingroup common_core_api_group */
/** @defgroup notification_group Notification
*
* @brief This group contains APIs that let you know when a signal's value changed.
*
* @details
*
* @ingroup common_core_api_group */
/** @defgroup schedule_management_group Schedule management
*
* @brief This group contains APIs that help you manage schedules table in master node only.
*
* @details
*
* @ingroup common_core_api_group */
/** @defgroup interface_management_group Interface management
*
* @brief This group contains APIs that help you manage interface(s) in LIN node.
*
* @details
*
* @ingroup common_core_api_group */
/** @defgroup call_out_group User provided call-outs
*
* @brief This group contains APIs which may be called from within the LIN module in order to enable/disable LIN communication interrupts.
*
* @details
*
* @ingroup common_core_api_group */

/** @defgroup commontl_api_group Common API 
* @brief contains APIs that used for both protocols LIN2.1 and J2602
*
* @ingroup transport_group */
/** @defgroup lin21tl_api_group LIN 2.1 specific API
* @brief contains APIs that only used for LIN 2.1 protocol
*
* @ingroup transport_group */
/** @defgroup j2602tl_api_group J2602 specific API
* @brief contains APIs that only used for J2602 protocol
*
* @ingroup transport_group */

/** @defgroup node_configuration_group Node configuration
*
* @brief This group contains APIs that used for node configuration purpose.
*
* @details 
*
* @ingroup lin21tl_api_group */
/** @defgroup node_identification_group Node identification
*
* @brief This group contains API that used for node identification purpose.
*
* @details 
*
* @ingroup lin21tl_api_group */

/** @defgroup initialization_group Initialization
*
* @brief Initialize transport layer (queues, status, ...).
*
* @details
*
* @ingroup commontl_api_group */
/** @defgroup raw_api_group Raw API
*
* @brief The raw API is operating on PDU level and it is typically used to gateway PDUs 
* between CAN and LIN. 
*
* @details  Usually, a FIFO is used to buffer PDUs in order to handle the 
* different bus speeds.
*
* @ingroup commontl_api_group */
/** @defgroup cooked_api_group Cooked API
*
* @brief Cooked processing of diagnostic messages manages one complete message at a time.
*
* @ingroup commontl_api_group */


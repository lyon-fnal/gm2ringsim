#ifndef G2MIGTRACE_SPINCONTROLLER_HH
#define G2MIGTRACE_SPINCONTROLLER_HH

/** @file spinController.hh

    Provides the declarations for the centralized spin tracking
    callback interface.
    
    @author Kevin Lynch 
    @date 2009-2011
 */

#include <boost/signals2.hpp>
#include <tr1/functional>
using std::tr1::function;

//class spinMessenger;

/** This typedef is a transparent "opaque type" to hide the
    exact implementation library. */
typedef boost::signals2::connection connection_t;

/** This enum is used to set callback ordering such that superior
    volumes are updated first, thus ensuring that lower volumes get
    the right field managers eventually.*/
enum e_connect_ordering {e_is_arc, e_arc_is_parent, e_vac_is_parent, e_dont_care};

/** This Meyer's singleton provides a centralized callback interface
    for enabling/disabling spin tracking in all the fields in the
    simulation.

    Spin tracking is the province of the field equations of motion,
    which are owned by the field managers which are in turn owned by
    logical volumes.  It's a horrible mess to go and change those
    field managers after they've been assigned, so we don't do that
    directly.  Instead, every class with an interest in swapping out
    field managers to deal with spin tracking registers at
    construction with this controller.  The spinMessenger provides the
    user interface to this callback framework.  The boost::signal2
    callback framework is used here, along with tr1::function and
    tr1::bind.  Because the field manager updates must be propagated
    to child volumes, the callbacks must be issued in specific order
    to ensure that volumes lower in the hierarchy eventually end up
    with the correct manager; for instance, the kicker must end up
    with the kicker field, and not just the storage field.

    Spin dependent decay control is provided by the physicsList and
    physicsListMessenger.
*/
class spinController {
public:
  static spinController& getInstance();

  void enable_spintracking();
  void disable_spintracking();
  bool query_spintracking() const { return spin_tracking_; }

  connection_t connect( e_connect_ordering, function<void(bool)> );
  void disconnect( connection_t );

private:
  spinController();
  ~spinController();

  // Don't implement!
  spinController(spinController const&);
  spinController& operator=(spinController const&);

  bool spin_tracking_;

  boost::signals2::signal<void(bool)> sig_;
  //  spinMessenger *sm_;

};

#endif // G2MIGTRACE_SPINCONTROLLER_HH
# Linux Synchronization Mechanisms - Interview Preparation

## 📚 Contents

This folder contains detailed explanations and working code examples for the three main synchronization mechanisms in Linux:

1. **[Mutex](./01_Mutex.md)** - Mutual Exclusion Lock
2. **[Semaphore](./02_Semaphore.md)** - Counting Synchronization Primitive
3. **[Condition Variable](./03_Condition_Variable.md)** - Wait for Specific Conditions

## 🎯 Interview Focus

Each file includes:

- ✅ Core concepts and theory
- ✅ Working code examples
- ✅ Common interview questions
- ✅ Real-world use cases
- ✅ Common pitfalls and bugs
- ✅ Performance considerations

## 🚀 How to Use

1. Start with `01_Mutex.md` - the most fundamental mechanism
2. Move to `02_Semaphore.md` - understand counting vs binary
3. Finish with `03_Condition_Variable.md` - advanced coordination

## 📝 Quick Reference

| Mechanism              | Use Case                 | Waiting Strategy     | Typical Interview Q             |
| ---------------------- | ------------------------ | -------------------- | ------------------------------- |
| **Mutex**              | Protect critical section | Sleep                | "Implement thread-safe counter" |
| **Semaphore**          | Limit concurrent access  | Sleep                | "Producer-consumer problem"     |
| **Condition Variable** | Wait for events          | Sleep with condition | "Blocking queue implementation" |

## 💡 Interview Tips

- Know when to use each mechanism
- Understand deadlock prevention
- Be able to write code on whiteboard
- Explain trade-offs between mechanisms
- Know the difference from spinlocks

---

**Good luck with your interview! 🎯**

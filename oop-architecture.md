
# وثيقة هندسة البرمجة كائنية التوجه (OOP Architecture) - Milestone 2
مشروع التجارة الإلكترونية بلغة C++

---

## 1. هيكلية الملفات وتطبيق فصل الواجهة عن التنفيذ (Separation of Interface & Implementation)

التزاماً بالمعايير الأكاديمية وهيكلية C++ القياسية، تم فصل كل كلاس (Class) إلى ملفين أساسيين:
- **ملفات الترويسة (`.h` / Header Files):** تحتوى على التعريفات الرسمية للكلاس (Declarations / Prototypes) مع حماية الملف من التكرار باستخدام `#ifndef` و `#define`.
- **ملفات التنفيذ (`.cpp` / Source Files):** تحتوي على كتابة الأكواد والوظائف الفعلية (Definitions) باستخدام نطاق الكلاس `Scope Resolution Operator (::)`.

### المجلدات والهيكل البرمجي:
```text
src/
├── models/
│   ├── User.h / User.cpp
│   ├── Product.h / Product.cpp
│   ├── Category.h / Category.cpp
│   ├── Order.h / Order.cpp
│   └── OrderItem.h / OrderItem.cpp
├── services/
│   ├── CatalogManager.h / CatalogManager.cpp
│   ├── CartManager.h / CartManager.cpp
│   └── OrderManager.h / OrderManager.cpp
└── main.cpp

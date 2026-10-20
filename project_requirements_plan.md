# وثيقة المتطلبات وخطة المشروع (Milestone 1)
# Project Requirements & Project Plan Document

---

## 1. بيان المشكلة والهدف (Problem Statement & Scope)

### بالعربية (Arabic)
**المشكلة:**  
تواجه المتاجر التقليدية ومشاريع التجارة الإلكترونية الناشئة صعوبة في إدارة الكتالوج والمخزون وحساب إجمالي الطلبات بدقة مع ضمان استقرار البيانات عند حدوث خطأ أثناء إجراء عملية الشراء (Checkout). كما أن غياب الآليات المباشرة للتحقق من كميات المخزون يؤدي إلى مشاكل مثل البيع المفرط (Overselling) أو تسجيل أسعار وكميات خاطئة.

**الهدف من النظام:**  
يقدم هذا المشروع نظام متجر إلكتروني (E-Commerce Console System) مبني بلغة C++ يوفر تجربة بيع وشراء آمنة ومستقرة. يضمن النظام إدارة كاملة للكتالوج والمخزون من قبل المشرف (Admin)، ويمكّن العملاء (Customers) من تصفح المنتجات، البحث، إدارة السلة، وإجراء التفقّد (Checkout) مع وجود إعادة تحقق تلقائية (Double Validation) لحماية بيانات المخزون وإنشاء لقطة مرجعية ثابتة (Snapshot) لتفاصيل الطلب.

### English
**Problem Statement:**  
Traditional retail and emerging e-commerce platforms struggle with accurate catalog and inventory management, computing totals reliably, and maintaining data consistency during checkouts. The lack of strict stock validation leads to overselling, invalid pricing, and corrupted order history.

**Project Purpose:**  
This project delivers a C++ based E-Commerce Console Application that ensures a secure, transactional, and robust shopping experience. It empowers Administrators to manage products and stock while enabling Customers to browse, search, manage cart items, and perform secure checkouts with double stock re-validation and historical order snapshots.

---

## 2. المصطلحات والافتراضات (Terminology & Assumptions)

### بالعربية (Arabic)
1. **حالات الطلب (Order Statuses):**
   - `Completed`: تم تأكيد الطلب بنجاح وتحديث المخزون.
   - `Cancelled`: حالة مخصصة للطلبات الملغاة أو التالفة (موجودة كقيمة مجردة في النظام لدعم التوسعات المستقبلية).
2. **لقطة الطلب (Order Snapshot):**
   - عند إتمام الشراء، يتم حفظ تفاصيل المنتجات (الاسم، السعر، والكمية) كما كانت لحظة الشراء. أي تعديل مستقبلي على سعر أو اسم المنتج في الكتالوج لا يؤثر على سجلات الطلبات القديمة.
3. **المنتجات الموقوفة (Discontinued Products):**
   - المنتجات الموقوفة أو المحذوفة تظل قائمة في سجلات الطلبات السابقة عبر الـ Snapshot ولا تظهر في كتالوج البحث للشراء الحالي.
4. **تمثيل الأسعار والكميات (Pricing & Quantities):**
   - الأسعار تُحسب بأرقام عشرية موجبة (`double` أو `float`) وتمنع القيم السالبة.
   - الكميات تُحسب بأعداد صحيحة موجبة فقط (`non-negative integers`).

### English
1. **Order Statuses:**
   - `Completed`: Order successfully processed and stock updated.
   - `Cancelled`: Reserved state for legacy or future order cancellation flows.
2. **Order Snapshot:**
   - At checkout, product details (Name, Unit Price, Quantity) are captured permanently. Future catalog edits do not alter historic orders.
3. **Discontinued Items:**
   - Inactive/deleted items remain preserved in prior order histories via snapshots but are hidden from active catalog browsing.
4. **Pricing & Quantity Representation:**
   - Prices are positive floating-point numbers; quantities are non-negative integers. Negative values are strictly rejected.

---

## 3. المتطلبات الوظيفية والمعيارية (Functional Requirements)

| المعرف (ID) | الوصف الوظيفي (Description) | معيار القبول والاختبار (Acceptance / Testability Criterion) |
| :--- | :--- | :--- |
| **01-FR** | عرض كتالوج المنتجات (Catalog Browsing) | يستطيع المستخدم عرض جميع المنتجات المتاحة مع إظهار الاسم، السعر، والكمية المتاحة. |
| **02-FR** | البحث عن المنتجات (Product Search) | إمكانية البحث عن المنتجات بالاسم أو الفئة واسترجاع النتائج المطابقة فقط. |
| **03-FR** | إدارة السلة (Cart Management) | إضافة، تعديل كمية، وحذف العناصر من السلة مع منع اختيار كمية تتجاوز المتاح بالمخزون. |
| **04-FR** | حساب إجمالي السلة (Cart Calculation) | حساب الإجمالي الدقيق للأسعار بضرب السعر في الكمية لكل عنصر وإضافة الضرائب/الخصومات إن وجدت. |
| **05-FR** | عملية الشراء والتحقق (Checkout & Validation) | إعادة التحقق من المخزون لحظة الـ Checkout؛ وفي حال عدم كفاية المخزون تلغى العملية دون تعديل البيانات. |
| **06-FR** | تسجيل لقطة الطلب (Order Snapshot Creation) | حفظ تفاصيل الطلب بشكل دائم بأسعار وأسماء اللحظة التي تم فيها الشراء. |
| **07-FR** | إدارة المنتجات للـ Admin (Admin Catalog Rules) | إمكانية إضافة منتج جديد، تعديل بياناته، أو حذفه مع منع الأسعار السالبة والكميات غير الصحيحة. |
| **08-FR** | إدارة المخزون للـ Admin (Admin Stock Control) | تحديث كميات المخزون مباشرة مع إرسال تنبيهات عند وصول المنتج لمستوى مخزون صفر. |
| **09-FR** | حفظ البيانات واسترجاعها (Persistence) | حفظ واسترجاع بيانات الكتالوج والطلبات من وإلى الملفات (`products.txt`, `orders.txt`). |

---

## 4. خطة المراحل الخمس (Initial Milestone Plan)

| المرحلة (Milestone) | النطاق والمخرجات (Scope & Deliverables) | الحالة (Status) |
| :--- | :--- | :--- |
| **Milestone 1** | إعداد وثيقة المتطلبات، المصطلحات، بيان المشكلة، وخطة المشروع المبدئية. | **قيد الإنجاز (In Progress)** |
| **Milestone 2** | صياغة حالات الاستخدام (Use Cases)، معايير القبول (Acceptance Criteria)، وتتبع المتطلبات (Traceability Matrix). | **التالي (Pending)** |
| **Milestone 3** | توثيق النموذج العلائقي للبيانات (Relational Data Model) وربطه مع ملفات التخزين (`products.txt` & `orders.txt`). | **مجدول (Scheduled)** |
| **Milestone 4** | إعداد خطة الاختبار الشاملة (Test Plan) لربط الـ 81 اختباراً المكتوبة بالمتطلبات الوظيفية. | **مجدول (Scheduled)** |
| **Milestone 5** | مراجعة الكود النهائي، إعداد الـ README الشامل، وتنظيف سجل التغييرات في Git (Commit History). | **مجدول (Scheduled)** |

---

## 5. القرارات التي تحتاج موافقة المدرب (Pending Decision Register)

1. **طريقة تخزين البيانات (Persistence Method):**
   - *القرار الحالي:* استخدام الملفات النصية المباشرة (`products.txt`, `orders.txt`).
   - *السؤال للمدرب:* هل اعتماد التخزين عبر الملفات مقبول ومستوفٍ لمتطلبات الدفعة أم يلزم الربط الفعلي بمحرك قواعد بيانات مثل SQLite؟
2. **توثيق استخدام الذكاء الاصطناعي (AI Assistance Transparency):**
   - *السؤال للمدرب:* هل يتطلب المستند تخصيص قسم محدد لتوضيح الأجزاء التي تم الاستعانة بالـ AI في مراجعتها وتطويرها؟
3. **تحديث سجل التغييرات في Git (Git Commit History):**
   - *السؤال للمدرب:* نظرًا لأن الرفع السابق تم دفعة واحدة في المتصفح، هل يُطلب إعادة هيكلة الـ Commits إلى سجل تدرجي تفصيلي يُظهر جميع المراحل؟
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct Product {
    char name[100];
    double price;
    int popularity;
    char category[100];
    int height;
    struct Product *left, *right;
} *root = NULL;
struct Product temp_products[1000];
int temp_count;
int sort_order = 1;  // 1 for High to Low, -1 for Low to High

int height(struct Product *N) {
    struct Product *p = N;
    if(p==NULL){
    	return 0;
	}
    return p->height;
}

int max(int a, int b) {
    int p = a, q = b;
    return (p > q) ? p : q;
}

struct Product* rightRotate(struct Product *y) {
    struct Product *p, *q, *t;
    p = y;
    q = p->left;
    t = q->right;
    q->right = p;
    p->left = t;
    p->height = max(height(p->left), height(p->right)) + 1;
    q->height = max(height(q->left), height(q->right)) + 1;
    return q;
}

struct Product* leftRotate(struct Product *x) {
    struct Product *p, *q, *t;
    p = x;
    q = p->right;
    t = q->left;
    q->left = p;
    p->right = t;
    p->height = max(height(p->left), height(p->right)) + 1;
    q->height = max(height(q->left), height(q->right)) + 1;
    return q;
}

int getBalance(struct Product *N) {
    struct Product *p = N;
    if (p == NULL)
        return 0;
    return height(p->left) - height(p->right);
}

struct Product* insertProduct(struct Product* node, char name[], double price, int popularity, char category[]) {
    struct Product *p, *q;
    p = (struct Product*) malloc(sizeof(struct Product));
    if (node == NULL) {
        strcpy(p->name, name);
        strcpy(p->category, category);
        p->price = price;
        p->popularity = popularity;
        p->left = NULL;
        p->right = NULL;
        p->height = 1;
        return p;
    }
    q = node;
    if (price < q->price)
        q->left = insertProduct(q->left, name, price, popularity, category);
    else if (price > q->price)
        q->right = insertProduct(q->right, name, price, popularity, category);
    else
        return q;
        
    q->height = 1 + max(height(q->left), height(q->right));
    int balance = getBalance(q);
    if (balance > 1 && price < q->left->price)
        return rightRotate(q);
    if (balance < -1 && price > q->right->price)
        return leftRotate(q);
    if (balance > 1 && price > q->left->price) {
        q->left = leftRotate(q->left);
        return rightRotate(q);
    }
    if (balance < -1 && price < q->right->price) {
        q->right = rightRotate(q->right);
        return leftRotate(q);
    }
    return q;
}

void displayAllFromTemp() {
    int i;
	for (i = 0; i < temp_count; i++) {
        printf("Name: %s | Price: %.2lf | Popularity: %d | Category: %s\n", temp_products[i].name, temp_products[i].price, temp_products[i].popularity, temp_products[i].category);
    }
}

void inOrder(struct Product* root) {
    struct Product *q = root;
    if (q != NULL) {
        inOrder(q->left);
        printf("Name: %s | Price: %.2lf | Popularity: %d | Category: %s\n", q->name, q->price, q->popularity, q->category);
        inOrder(q->right);
    }
}

void reverseInOrder(struct Product* root) {
    struct Product *q = root;
    if (q != NULL) {
        reverseInOrder(q->right);
        printf("Name: %s | Price: %.2lf | Popularity: %d | Category: %s\n", q->name, q->price, q->popularity, q->category);
        reverseInOrder(q->left);
    }
}
int comparePopularity(const void *a, const void *b) {
    struct Product *productA = (struct Product *)a;
    struct Product *productB = (struct Product *)b;
    return sort_order * (productB->popularity - productA->popularity);
}

void inOrderStore(struct Product* root) {
    if (root) {
        inOrderStore(root->left);
        temp_products[temp_count++] = *root;
        inOrderStore(root->right);
    }
}

void displayByPopularity(int order) {
    temp_count = 0;  // Reset the counter
    inOrderStore(root);  // Fill the temp_products array
    sort_order = order;  // Set the global sorting order
    qsort(temp_products, temp_count, sizeof(struct Product), comparePopularity);  // Sort by popularity
    int i;
    for (i = 0; i < temp_count; i++) {
        printf("Name: %s | Price: %.2lf | Popularity: %d | Category: %s\n", temp_products[i].name, temp_products[i].price, temp_products[i].popularity, temp_products[i].category);
    }
}

int containsSubstring(char* str, char* substr) {
    int strLen = strlen(str);
    int substrLen = strlen(substr);
    int i,j;
    for (i = 0; i <= strLen - substrLen; i++) {
        for (j = 0; j < substrLen; j++) {
            if (str[i + j] != substr[j]) {
                break;
            }
        }
        if (j == substrLen) {
            return 1; // Found the substring
        }
    }
    return 0;
}

struct Product* findProductByName(struct Product* node, char* productName) {
    struct Product *p, *q;
    q = node;
    if (q == NULL) return NULL;
    if (containsSubstring(q->name, productName)) {
        return q;
    }
    p = findProductByName(q->left, productName);
    if (p) return p;
    return findProductByName(q->right, productName);
}

void recommendByCategory(struct Product* node, char* category) {
    struct Product *q = node;
    if (q == NULL) return;
    if (strcmp(q->category, category) == 0) {
        printf("Name: %s | Price: %.2lf | Popularity: %d | Category: %s\n", q->name, q->price, q->popularity, q->category);
    }
    recommendByCategory(q->left, category);
    recommendByCategory(q->right, category);
}

void searchProduct(struct Product* node, char* searchStr) {
    if (node == NULL) return;
    if (containsSubstring(node->name, searchStr)) {
        printf("Name: %s | Price: %.2lf | Popularity: %d | Category: %s\n", node->name, node->price, node->popularity, node->category);
    }
    else{
    	printf("Product not Found!!!\n");
    	return;
	}
    searchProduct(node->left, searchStr);
    searchProduct(node->right, searchStr);
}

struct Product* loadProductsFromCSV(const char* filePath) {
    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        printf("Error: Cannot open file.\n");
        return NULL;
    }
    char line[256];
    fgets(line, sizeof(line), file);
    while (fgets(line, sizeof(line), file)) {
        char *name = strtok(line, ",");
        char *price_str = strtok(NULL, ",");
        char *popularity_str = strtok(NULL, ",");
        char *category = strtok(NULL, ",");
        size_t len = strlen(category);
        while(len > 0 && (category[len - 1] == '\n' || category[len - 1] == ' ')) {
            category[--len] = '\0';
        }
        double price = atof(price_str);
        int popularity = atoi(popularity_str);
        root = insertProduct(root, name, price, popularity, category);
        // Populate temp_products array with the current product
        strcpy(temp_products[temp_count].name, name);
        strcpy(temp_products[temp_count].category, category);
        temp_products[temp_count].price = price;
        temp_products[temp_count].popularity = popularity;
        temp_count++;
    }
    fclose(file);
    return root;
}

int main() {
    root = loadProductsFromCSV("products.csv");
    int choice;
    do {
        printf("\nE-Commerce Platform:\n");
        printf("1. Display All Products\n");
        printf("2. View Products Sorted By Price (Low to High)\n");
        printf("3. View Products Sorted By Price (High to Low)\n");
        printf("4. View Products Based On Popularity\n");
        printf("5. Product Recommendations\n");
        printf("6. Search Product\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                displayAllFromTemp();  // Display products from temp_products
                break;
            case 2:
                printf("\nProducts Sorted By Price (Low to High):\n");
                inOrder(root);
                break;
            case 3:
                printf("\nProducts Sorted By Price (High to Low):\n");
                reverseInOrder(root);
                break;
            case 4:
                printf("\nProducts Sorted By Popularity:\n");
                printf("1. High to Low\n");
                printf("2. Low to High\n");
                printf("Enter Your Choice: ");
                int popOrderChoice;
                scanf("%d", &popOrderChoice);
                if (popOrderChoice == 1) {
                   displayByPopularity(1);
                } else if (popOrderChoice==2) {
                   displayByPopularity(-1);
                } else {
                	printf("\nInvalid Input\n");
				}
                break;
            case 5:
                printf("Enter product name for recommendations: ");
                char productName[100];
                scanf("%s", productName);
                struct Product* foundProduct = findProductByName(root, productName);
                if (foundProduct) {
                   printf("\nRecommendations in the category of '%s':\n", foundProduct->category);
                   recommendByCategory(root, foundProduct->category);
                } else {
                   printf("Product not found!\n");
                }
                break;
            case 6:
                printf("Enter product name to search: ");
                char searchName[100];
                scanf("%s", searchName);
                searchProduct(root, searchName);
                break;
            case 7:
                printf("Exiting...\n");
                return 1;
            default:
                printf("Invalid choice! Please try again.\n");
                break;
        }
    } while(1);
    return 1;
}
